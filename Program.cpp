#include "stdafx.h"
#include "./Systems/Device.h"
#include "./Scenes/Scene.h"

#include "./Scenes/Stage1.h"

SceneValues* values;
vector<Scene*> scenes;

void InitScene() {
	values = new SceneValues();
	values->MainCamera = new Camera();
	D3DXMatrixIdentity(&values->Projection);

	scenes.push_back(new Stage1(values));
}

void DestroyScene(){
	for (auto scene : scenes)
		SAFE_DELETE(scene);

	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(values);
}

void Update() {
	values->MainCamera->Update();
	D3DXMatrixOrthoOffCenterLH(&values->Projection, 0, (float)Width/3, 0, (float)Height/3, -10, 10);

	for (auto scene : scenes)
		scene->Update();
}

void Render() {
	D3DXCOLOR bgColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		// ImGui::LabelText("FPS", "%.0f", ImGui::GetIO().Framerate);
		for (auto scene : scenes)
			scene->Render();
	}
	ImGui::Render();
	SwapChain->Present(0, 0);
}

