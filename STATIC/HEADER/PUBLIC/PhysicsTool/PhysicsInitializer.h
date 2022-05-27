/*
*
* < PhysicsIntializer.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "SAMPLES/PhysicsEngine.h"

/*
* [ Object Type List ]
* 1. Box
* 2. Sphere
* 
* [ Color List ]
* 1. red
* 2. green
* 3. blue
* 
* [ Object Option List ]
* 1. SphereOpt
* 2. BoxOpt
* 
* how to generate an object?
* > std::unique_ptr<[ObjectType]> [Name] = nullptr;
* 
* how to create an sphere object option?
* > SphereOpt sphOpt(0.0f, 10.0f, 0.0f, 2.0f, 2.0f, 2.0f, "blue"); Sph1 = sphere(sphOpt);
* 
* 
*/

namespace orangelie {

	namespace PhysicsTool {

		class PhysicsIntializer : public PhysicsEngine {
		public:
			PhysicsIntializer() {}
			virtual ~PhysicsIntializer() {}

		protected:								// Mutable
			virtual void startup() {
				/* Do Something */

				SetCameraSpeed(10.0f);

				SphereOpt sphOpt(0.0f, 10.0f, 0.0f, 2.0f, 2.0f, 2.0f, "blue");
				Sph1 = sphere(sphOpt);

				BoxOpt boxOpt(0.0f, 5.0f, 0.0f, 2.0f, 2.0f, 2.0f, "green");
				Box1 = box(boxOpt);
			}

			virtual void render(float dt) {
				static float t = 0.0f;
				t += dt;

				Sph1->SetPosition(t, 10.0f, 0.0f);
				Box1->SetPosition(t, 5.0f, 0.0f);
			}

		protected:								// Unmutable
			virtual void OnResize() {
				PhysicsEngine::OnResize();


			}

			virtual void init() {
				PhysicsEngine::init();


			}

			virtual void draw(float dt) override {
				PhysicsEngine::draw(dt);


			}

			virtual void MouseDown(WPARAM btnState, int x, int y) override {
				PhysicsEngine::MouseDown(btnState, x, y);


			}

			virtual void MouseUp(WPARAM btnState, int x, int y) override {
				PhysicsEngine::MouseUp(btnState, x, y);


			}

			virtual void MouseMove(WPARAM btnState, int x, int y, float dt) override {
				PhysicsEngine::MouseMove(btnState, x, y, dt);


			}

		private:
			std::unique_ptr<Sphere> Sph1 = nullptr;
			std::unique_ptr<Box> Box1 = nullptr;

		};
	}
}