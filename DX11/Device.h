#pragma once
class Device
{
public:

	HRESULT Init(const GWindow& window);

	ComPtr < ID3D11Device> GetDevice()						{ return _device; }
	ComPtr < ID3D11DeviceContext> GetContext()				{ return _context; }
	ComPtr < IDXGISwapChain> GetSwapChain()					{ return _swapChain; }
	
	ComPtr<ID3D11RenderTargetView> GetRenderTargetView()	{ return _renderTargetView; }
	ComPtr<ID3D11Texture2D> GetDepthStencil()				{ return _depthStencil; }
	ComPtr<ID3D11DepthStencilView> GetDepthStencilView()	{ return _depthStencilView; }
	
	ComPtr<ID3D11RenderTargetView> GetMultipleRenderTargetView() { return _multipleRenderTargetView; }
	ComPtr<ID3D11ShaderResourceView> GetMultipleRenderTargetResource() { return _multipleRenderTargetSRV; }
	ComPtr<ID3D11DepthStencilView> GetMultipleRenderDepthStencilView() { return _multipleRenderDepthStencilView; }


private:
	ComPtr<ID3D11Device>				_device = NULL;
	ComPtr<ID3D11DeviceContext>		_context = NULL;
	
	ComPtr<IDXGISwapChain>			_swapChain = NULL;
	ComPtr<ID3D11RenderTargetView>		_renderTargetView = NULL;


	ComPtr<ID3D11Texture2D>				_depthStencil = NULL;
	ComPtr<ID3D11DepthStencilView>		_depthStencilView = NULL;


	ComPtr<ID3D11Texture2D>				_multipleRenderTarget;
	ComPtr<ID3D11RenderTargetView>		_multipleRenderTargetView;
	ComPtr<ID3D11ShaderResourceView>	_multipleRenderTargetSRV;

	ComPtr<ID3D11Texture2D>				_multipleDepthStencil = NULL;
	ComPtr<ID3D11DepthStencilView>		_multipleRenderDepthStencilView = NULL;
};

