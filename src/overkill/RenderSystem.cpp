#include "RenderSystem.hpp"

namespace OK
{

void OK::RenderGroup::add(Rendered item)
{
    rendered_items.push_back(item);
}

}