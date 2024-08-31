#include "shared.h"
#include "mesh.h"
#include "transform.h"

// TODO: don't do too much before implementing skeletal transforms
class Object
{
  public:
    void init(
        std::function<const TickData&> tick_function,
        PhongModel* model,
        Transform initial_transform
    );

    // draw the object onto the current fb
    void draw(const glm::mat4& view, const glm::mat4& proj);

    // update object
    void tick(const TickData& tick_data);

  private:
    Transform transform;
    PhongModel* model; // many objects can have the same mode 
    std::function<const TickData&> tick_function;
};
