#pragma once

namespace GCE
{
	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual unsigned int getRendererID() const = 0;

		virtual void setData(void* data, unsigned int size) = 0;

		virtual void bind(unsigned int slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> create(const std::string& path);
		static Ref<Texture2D> create(unsigned int width, unsigned int height);

	};
}