#include "MaterialD11.h"
#include "../../Platform/Application.h"
#include "TextureD11.h"
#include "SamplerStateD11.h"
#include "GraphicsMgrD11.h"

void Excalibur::MaterialD11::Initialize()
{

}

void Excalibur::MaterialD11::Finalize()
{

}

void Excalibur::MaterialD11::Apply(ConstantBuffer cb)
{
	auto mgrd11 = (GraphicsMgrD11*)GApp->mGraphicsManager;
	m_Shader->Use();
	for (auto pair : m_Parameters) {
		if (pair.first == "color") {
			cb.debugColor = pair.second;
		}
	}
	m_Shader->SetConstantBuffer(cb);

	/*for (auto pair : mTextures) {
		if (pair.first == "tBaseMap") {
			auto texture = pair.second;
			auto sampler = mSamplerState[pair.first];
			if (texture && sampler) {
				auto _texuter = dynamic_pointer_cast<TextureD11>(texture);
				auto _sampler = dynamic_pointer_cast<SamplerStateD11>(sampler);
				mgrd11->m_deviceContext->PSSetShaderResources(0, 1, &(_texuter->mView));
				mgrd11->m_deviceContext->PSSetSamplers(0, 1, &(_sampler->m_sampleState));
			}
		}
	}*/
}
