#pragma once

typedef HRESULT(__stdcall* D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall* D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall* D3D11ClearRenderTargetViewHook) (ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]);

ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* renderTargetView = NULL;

DWORD_PTR* pSwapChainVtable = NULL;
DWORD_PTR* pDeviceContextVTable = NULL;

void* phookD3D11Present = NULL;
void* phookD3D11DrawIndexed = NULL;
void* phookD3D11ClearRenderTargetView = NULL;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool firstRun = false;
WNDPROC oWndProc = NULL;
bool menuOpenned = false;

class CRenderEngine {
public:
	void getD3D11VTables() {
		HWND hWnd = FindWindowA("grcWindow", nullptr);
		IDXGISwapChain* pSwapChain;

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = ((GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, NULL, &pContext)))
		{
			MessageBoxA(hWnd, "Failed to create directX device and swapchain!", "uBoos?", MB_ICONERROR);
			return;
		}

		pSwapChainVtable = (DWORD_PTR*)pSwapChain;
		pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];

		pDeviceContextVTable = (DWORD_PTR*)pContext;
		pDeviceContextVTable = (DWORD_PTR*)pDeviceContextVTable[0];
	}

	void stop() {
		SetWindowLongPtr(FindWindowA("grcWindow", nullptr), GWLP_WNDPROC, (LONG_PTR)oWndProc);
	}
};

extern CRenderEngine* renderEngine = new CRenderEngine();

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (menuOpenned && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) {
	return ((D3D11DrawIndexedHook)phookD3D11DrawIndexed)(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

void __stdcall hookD3D11ClearRenderTargetView(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]) {
	return ((D3D11ClearRenderTargetViewHook)phookD3D11ClearRenderTargetView)(pContext, pRenderTargetView, ColorRGBA);
}

HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!firstRun) {
		pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);
		oWndProc = (WNDPROC)SetWindowLongPtr(FindWindowA("grcWindow", nullptr), GWLP_WNDPROC, (LONG_PTR)WndProc);

		ID3D11Texture2D* renderTargetTexture = nullptr;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
		{
			pDevice->CreateRenderTargetView(renderTargetTexture, NULL, &renderTargetView);
			renderTargetTexture->Release();
		}

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(FindWindowA("grcWindow", nullptr));
		ImGui_ImplDX11_Init(pDevice, pContext);
		firstRun = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) < 0)
		menuOpenned = !menuOpenned;

	if (menuOpenned) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = true;

		ImGui::Begin("ImGui Window");
		ImGui::End();
	}
	else {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = false;
	}

	if (auto localPed = functions::getLocalPlayer()) {
		playerEsp->drawPlayers();
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &renderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return ((D3D11PresentHook)phookD3D11Present)(pSwapChain, SyncInterval, Flags);
}