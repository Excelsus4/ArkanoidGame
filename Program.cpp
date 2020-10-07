#include "stdafx.h"
#include "./Systems/Device.h"
#include "./Scenes/Scene.h"
#include "./Viewer/Freedom.h"

#include "./Scenes/Stage1.h"
#include "./Scenes/Stage2.h"

SceneValues* values;
vector<Scene*> scenes;

void InitScene() {
	values = new SceneValues();
	values->MainCamera = new Freedom();
	D3DXMatrixIdentity(&values->Projection);

	scenes.push_back(new Stage1(values));
	//scenes.push_back(new Stage2(values));
}

void DestroyScene(){
	for (auto scene : scenes)
		SAFE_DELETE(scene);

	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(values);
}

void Update() {
	values->MainCamera->Update();
	D3DXMatrixOrthoOffCenterLH(&values->Projection, 0, (float)Width, 0, (float)Height, -10, 10);

	for (auto scene : scenes)
		scene->Update();
}

void Render() {
	D3DXCOLOR bgColor = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		// ImGui::LabelText("FPS", "%.0f", ImGui::GetIO().Framerate);
		for (auto scene : scenes)
			scene->Render();
	}
	ImGui::Render();

	DirectWrite::GetDC()->BeginDraw();
	{
		wstring text = L"";

		RECT rect;
		rect.left = 20;
		rect.top = 0;
		rect.right = 600;
		rect.bottom = 20;

		text = L"Frame Per Second : " + to_wstring((int)ImGui::GetIO().Framerate);
		DirectWrite::RenderText(text, rect);

		rect.top += 20;
		rect.bottom += 20;

		text = L"Camera Position : ";
		text += to_wstring((int)values->MainCamera->Position().x);
		text += L", ";
		text += to_wstring((int)values->MainCamera->Position().y);
		DirectWrite::RenderText(text, rect);
	}

	DirectWrite::GetDC()->EndDraw();
	SwapChain->Present(0, 0);
}

