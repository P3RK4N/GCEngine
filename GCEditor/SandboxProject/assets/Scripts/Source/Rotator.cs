using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GCE;

namespace Sandbox
{
    public class Rotator : Entity
    {
        private TransformComponent m_Transform;
        private Rigidbody2DComponent m_Rigidbody;

        private float time;
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");

            m_Transform = GetComponent<TransformComponent>();
            m_Rigidbody = GetComponent<Rigidbody2DComponent>();

            time = 0.0f;
        }

        void OnUpdate(float ts)
        {
            time += ts;
            m_Transform.Translation = new Vector3((float)Math.Sin(time*3.0f), (float)Math.Cos(time*3.0f), 0.1f);
            Console.WriteLine($"Time - {time} , Position - {m_Transform.Translation.X} {m_Transform.Translation.Y} {m_Transform.Translation.Z}");
        }

    }
}
