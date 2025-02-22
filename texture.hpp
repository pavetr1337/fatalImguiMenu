#include <D3dx9tex.h>
#include <tchar.h>
#pragma comment(lib, "D3dx9")

IDirect3DTexture9* LoadTextureFromMemory(IDirect3DDevice9* device, const unsigned char* image_data, size_t image_size) {
	IDirect3DTexture9* texture = nullptr;
	D3DXIMAGE_INFO info;

	HRESULT hr = D3DXCreateTextureFromFileInMemory(
		device,
		image_data,
		image_size,
		&texture
	);
	if (FAILED(hr)) {
		MessageBox(NULL, _T("Failed to create texture from memory"), _T("Error"), MB_OK);
	}
	return texture;
}