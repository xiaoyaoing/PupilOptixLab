#pragma once

namespace optix_util {
struct TriAreaEmitter {
    cuda::Texture radiance;

    float area;

    struct {
        struct {
            float3 pos;
            float3 normal;
            float2 tex;
        } v0, v1, v2;
    } geo;

    CUDA_HOSTDEVICE EmitterSampleRecord SampleDirect(LocalGeometry hit_geo, float2 xi) const noexcept {
        EmitterSampleRecord ret{};
        float3 t = optix_util::UniformSampleTriangle(xi.x, xi.y);
        float3 position = geo.v0.pos * t.x + geo.v1.pos * t.y + geo.v2.pos * t.z;
        float3 normal = normalize(geo.v0.normal * t.x + geo.v1.normal * t.y + geo.v2.normal * t.z);
        auto tex = geo.v0.tex * t.x + geo.v1.tex * t.y + geo.v2.tex * t.z;
        ret.radiance = radiance.Sample(tex);

        ret.wi = normalize(position - hit_geo.position);
        float NoL = dot(hit_geo.normal, ret.wi);
        float LNoL = dot(normal, -ret.wi);
        if (NoL > 0.f && LNoL > 0.f) {
            float distance = length(position - hit_geo.position);
            ret.pdf = distance * distance / (LNoL * area);
            ret.distance = distance;
        }

        return ret;
    }

    CUDA_HOSTDEVICE EmitEvalRecord Eval(LocalGeometry emit_local_geo, float3 scatter_pos) const noexcept {
        EmitEvalRecord ret{};
        float3 dir = normalize(scatter_pos - emit_local_geo.position);
        float LNoL = dot(emit_local_geo.normal, dir);
        if (LNoL > 0.f) {
            float distance = length(scatter_pos - emit_local_geo.position);
            ret.pdf = distance * distance / (LNoL * area);
            ret.radiance = radiance.Sample(emit_local_geo.texcoord);
        }
        return ret;
    }
};
}// namespace optix_util