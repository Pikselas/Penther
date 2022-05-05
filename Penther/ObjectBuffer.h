#pragma once
#include<memory>
#include<d3d11.h>

template<class Canvas3D>
class ObjectBuffer
{
	private:
		unsigned int COUNT = 0;
		unsigned int STRIDE = 0;
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> VERTEX_BUFFER;
		Microsoft::WRL::ComPtr<ID3D11Buffer> INDEX_BUFFER;
	private:
		bool IndexedDrawAble = false;
	public:
		ObjectBuffer() = default;
		ObjectBuffer(const Canvas3D& c3d , const void * vertices, unsigned int stride , unsigned int total_vertices)
		{
			Set( c3d ,vertices , stride, total_vertices);
		}
	public:
		void Set(const Canvas3D& c3d , const void * vertices , unsigned int stride ,unsigned int total_vertices)
		{
			COUNT = total_vertices;
			D3D11_BUFFER_DESC vertex_bd;
			D3D11_SUBRESOURCE_DATA vertex_sd;

			vertex_bd.ByteWidth = (UINT)stride * total_vertices;
			vertex_bd.Usage = D3D11_USAGE_DEFAULT;
			vertex_bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_bd.CPUAccessFlags = 0u;
			vertex_bd.MiscFlags = 0u;
			vertex_bd.StructureByteStride = stride;

			vertex_sd.pSysMem = vertices;

			STRIDE = stride;

			c3d.Device->CreateBuffer(&vertex_bd, &vertex_sd, &VERTEX_BUFFER);

		}
		void Set( const Canvas3D& c3d , const void* vertices, unsigned int stride, unsigned int total_vertices, const unsigned int* indices, unsigned int total_indices)
		{
			Set(c3d ,vertices, stride, total_vertices);
			
			IndexedDrawAble = true;

			COUNT = total_indices;
			
			D3D11_BUFFER_DESC index_bd;
			D3D11_SUBRESOURCE_DATA index_sd;

			index_bd.ByteWidth = sizeof(unsigned int) * total_indices;
			index_bd.Usage = D3D11_USAGE_DEFAULT;
			index_bd.CPUAccessFlags = 0u;
			index_bd.MiscFlags = 0u;
			index_bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			index_bd.StructureByteStride = sizeof(unsigned int);

			index_sd.pSysMem = indices;

			c3d.Device->CreateBuffer(&index_bd, &index_sd, &INDEX_BUFFER);
		}
		void Draw(ID3D11DeviceContext& Context) const
		{
			UINT offset = 0u;
			Context.IASetVertexBuffers(0u, 1u, VERTEX_BUFFER.GetAddressOf(), &STRIDE, &offset);

			if (IndexedDrawAble)
			{
				Context.IASetIndexBuffer(INDEX_BUFFER.Get(), DXGI_FORMAT_R32_UINT, 0u);
				Context.DrawIndexed(COUNT, 0u, 0u);
			}
			else
			{
				Context.Draw(COUNT, 0u);
			}
		}
};