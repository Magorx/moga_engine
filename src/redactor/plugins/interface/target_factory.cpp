#include "target_factory.h"
#include "target.h"


P::RenderTarget *TargetFactory::spawn(P::Vec2s size, P::RGBA color) const {
    auto ret = new Target({(double) size.x, (double) size.y});
    ret->clear(color);

    return ret;
}

P::RenderTarget *TargetFactory::from_pixels(P::Vec2s size, const P::RGBA *data) const {
    auto ret = new Target({(double) size.x, (double) size.y});
    ret->update(data);

    return ret;
}

P::RenderTarget *TargetFactory::from_file(const char *path) const {
    return new Target(path);
}
