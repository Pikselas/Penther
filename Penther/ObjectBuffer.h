#pragma once
#include<memory>
#include<d3d11.h>

class ObjectBuffer
{
	private:
		D3D11_BUFFER_DESC vertex_bd;
		D3D11_SUBRESOURCE_DATA vertex_sd;
		D3D11_BUFFER_DESC index_bd;
		D3D11_SUBRESOURCE_DATA index_sd;
	private:
		unsigned int COUNT = 0;
	private:
		std::unique_ptr<unsigned int[]> INDEX_BUFFER;
		std::unique_ptr<BYTE[]> VERTEX_BUFFER;
	private:
		bool IndexedDrawAble = false;
	public:
		ObjectBuffer() = default;
		ObjectBuffer(const void * vertices, unsigned int stride , unsigned int total_vertices) : index_bd{0},index_sd{0}
		{
			Set(vertices , stride, total_vertices);
		}
	public:
		void Set(const void * vertices , unsigned int stride ,unsigned int total_vertices)
		{
			COUNT = total_vertices;
			vertex_bd.ByteWidth = (UINT)stride * total_vertices;
			vertex_bd.Usage = D3D11_USAGE_DEFAULT;
			vertex_bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_bd.CPUAccessFlags = 0u;
			vertex_bd.MiscFlags = 0u;
			vertex_bd.StructureByteStride = stride;

			VERTEX_BUFFER = std::make_unique<BYTE[]>(vertex_bd.ByteWidth);

			std::memcpy(VERTEX_BUFFER.get(), vertices, vertex_bd.ByteWidth);

			vertex_sd.pSysMem = VERTEX_BUFFER.get();

		}
		void Set(const void* vertices, unsigned int stride, unsigned int total_vertices, const unsigned int* indices, unsigned int total_indices)
		{
			Set(vertices, stride, total_vertices);
			
			IndexedDrawAble = true;

			COUNT = total_indices;
			
			index_bd.ByteWidth = sizeof(unsigned int) * total_indices;
			index_bd.Usage = D3D11_USAGE_DEFAULT;
			index_bd.CPUAccessFlags = 0u;
			index_bd.MiscFlags = 0u;
			index_bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			index_bd.StructureByteStride = sizeof(unsigned int);

			INDEX_BUFFER = std::make_unique<unsigned int[]>(total_indices);
			std::memcpy(INDEX_BUFFER.get(), indices, sizeof(unsigned int) * total_indices);

			index_sd.pSysMem = INDEX_BUFFER.get();
		}
		void Draw(ID3D11Device& Device, ID3D11DeviceContext& Context) const
		{
			Microsoft::WRL::ComPtr<ID3D11Buffer> VBuffer;
			Device.CreateBuffer(&vertex_bd, &vertex_sd, &VBuffer);
			UINT stride = vertex_bd.StructureByteStride;
			UINT offset = 0u;
			Context.IASetVertexBuffers(0u, 1u, VBuffer.GetAddressOf(), &stride, &offset);

			if (IndexedDrawAble)
			{
				Microsoft::WRL::ComPtr<ID3D11Buffer> IndxBuff;
				Device.CreateBuffer(&index_bd, &index_sd, &IndxBuff);
				Context.IASetIndexBuffer(IndxBuff.Get(), DXGI_FORMAT_R32_UINT, 0u);

				Context.DrawIndexed(COUNT, 0u, 0u);
			}
			else
			{
				Context.Draw(COUNT, 0u);
			}
		}
};