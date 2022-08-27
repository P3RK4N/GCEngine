#include "GCEPCH.h"
#include "GCE/Scripting/ScriptEngine.h"

#include "GCE/Scripting/ScriptGlue.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

namespace GCE
{
	namespace Utils
	{
		static char* readBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint64_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = (uint32_t)size;
			return buffer;
		}

		static MonoAssembly* loadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = readBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			std::string pathString = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		void printAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				GCE_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* rootDomain = nullptr;
		MonoDomain* appDomain = nullptr;

		MonoAssembly* coreAssembly = nullptr;
		MonoImage* coreAssemblyImage = nullptr;

		MonoAssembly* appAssembly = nullptr;
		MonoImage* appAssemblyImage = nullptr;

		ScriptClass entityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> entityClasses;
		std::unordered_map<uint64_t, Ref<ScriptInstance>> entityInstances;

		Scene* sceneContext = nullptr;
	};

	static ScriptEngineData* s_ScriptData = nullptr;

	void ScriptEngine::init()
	{
		s_ScriptData = new ScriptEngineData();

		initMono();
		loadAssembly("resources/Scripts/GCEngine-ScriptCore.dll");
		loadAppAssembly("SandboxProject/assets/Scripts/Binaries/Sandbox.dll");
		loadAssemblyClasses();

		ScriptGlue::registerComponents();
		ScriptGlue::registerFunctions();

		s_ScriptData->entityClass = ScriptClass("GCE", "Entity", true);
	}

	void ScriptEngine::shutdown()
	{
		shutdownMono();
		delete s_ScriptData;
	}

	void ScriptEngine::shutdownMono()
	{
		 //mono_domain_unload(s_ScriptData->appDomain);
		s_ScriptData->appDomain = nullptr;

		// mono_jit_cleanup(s_ScriptData->rootDomain);
		s_ScriptData->rootDomain = nullptr;
	}

	void ScriptEngine::initMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("GCEJITRuntime");

		if(rootDomain == nullptr)
			return;

		s_ScriptData->rootDomain = rootDomain;
	}

	void ScriptEngine::loadAssembly(const std::filesystem::path& filepath)
	{
		// Create an App Domain
		char appDomainName[17] = "GCEScriptRuntime";
		s_ScriptData->appDomain = mono_domain_create_appdomain(appDomainName, nullptr);
		mono_domain_set(s_ScriptData->appDomain, true);

		// Move this maybe
		s_ScriptData->coreAssembly = Utils::loadMonoAssembly(filepath);
		s_ScriptData->coreAssemblyImage = mono_assembly_get_image(s_ScriptData->coreAssembly);
	}

	void ScriptEngine::loadAppAssembly(const std::filesystem::path& filePath)
	{
		s_ScriptData->appAssembly = Utils::loadMonoAssembly(filePath);
		s_ScriptData->appAssemblyImage = mono_assembly_get_image(s_ScriptData->appAssembly);
	}

	MonoObject* ScriptEngine::instantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_ScriptData->appDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	void ScriptEngine::onRuntimeStart(Scene* scene)
	{
		s_ScriptData->sceneContext = scene;
	}

	bool ScriptEngine::entityClassExists(const std::string& fullClassName)
	{
		return s_ScriptData->entityClasses.find(fullClassName) != s_ScriptData->entityClasses.end();
	}

	void ScriptEngine::onCreateEntity(Entity entity)
	{
		const auto& sc = entity.getComponent<ScriptComponent>();
		if (ScriptEngine::entityClassExists(sc.className))
		{
			Ref<ScriptInstance> instance = createRef<ScriptInstance>(s_ScriptData->entityClasses[sc.className], entity);
			s_ScriptData->entityInstances[(uint64_t)entity.getUUID()] = instance;
			instance->invokeOnCreate();
		}
	}

	void ScriptEngine::onUpdateEntity(Entity entity, Timestep ts)
	{
		UUID entityUUID = entity.getUUID();
		GCE_CORE_ASSERT(s_ScriptData->entityInstances.find((uint64_t)entityUUID) != s_ScriptData->entityInstances.end(), "Entity script instance doesn't exist");

		Ref<ScriptInstance> instance = s_ScriptData->entityInstances[(uint64_t)entityUUID];
		instance->invokeOnUpdate((float)ts);
	}

	Scene* ScriptEngine::getSceneContext()
	{
		return s_ScriptData->sceneContext;
	}

	void ScriptEngine::onRuntimeStop()
	{
		s_ScriptData->sceneContext = nullptr;

		s_ScriptData->entityInstances.clear();
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::getEntityClasses()
	{
		return s_ScriptData->entityClasses;
	}

	void ScriptEngine::loadAssemblyClasses(MonoAssembly* assembly)
	{
		s_ScriptData->entityClasses.clear();

		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(image, "GCE", "Entity");

		for(int i = 0; i < numTypes; i++)
		{
			unsigned int cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;

			if (strlen(nameSpace) != 0)
				fullName += fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);

			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (isEntity)
			{
				s_ScriptData->entityClasses[fullName] = createRef<ScriptClass>(nameSpace, name); 
			}
		}
	}

	void ScriptEngine::loadAssemblyClasses()
	{
		s_ScriptData->entityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_ScriptData->appAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_ScriptData->coreAssemblyImage, "GCE", "Entity");

		for(int i = 0; i < numTypes; i++)
		{
			unsigned int cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_ScriptData->appAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_ScriptData->appAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;

			if (strlen(nameSpace) != 0)
				fullName += fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(s_ScriptData->appAssemblyImage, nameSpace, name);

			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (isEntity)
			{
				s_ScriptData->entityClasses[fullName] = createRef<ScriptClass>(nameSpace, name); 
			}
		}
	}

	MonoImage* ScriptEngine::getCoreAssemblyImage()
	{
		return s_ScriptData->coreAssemblyImage;
	}

	

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->instantiate();

		m_Constructor = s_ScriptData->entityClass.getMethod(".ctor", 1);
		m_OnCreateMethod = scriptClass->getMethod("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->getMethod("OnUpdate", 1);

		{
			uint64_t entityID = entity.getUUID();
			void* param = &entityID;
			m_ScriptClass->invokeMethod(m_Instance, m_Constructor, &param);
		}
	}

	void ScriptInstance::invokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->invokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::invokeOnUpdate(float ts)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->invokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}



	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		: m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(isCore ? s_ScriptData->coreAssemblyImage : s_ScriptData->appAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::instantiate()
	{
		return ScriptEngine::instantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::getMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::invokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

}