#include "Renderable.hpp"

using namespace andromeda::graphics;
using namespace andromeda::math;
using namespace andromeda::util;

Renderable::Renderable():
		direct_instance_list(DIRECT_INSTANCE_CAPCITY),textures(Texture2D::getMaxTextureUnit()),vertex_data(1024),element_data(1024)
{
	setDrawStrategy(DrawStrategy::NORMAL);
}

RenderableInstance& Renderable::instance(const char* id)
{
	if(id)
		return named_instance_map[id];
	else
		return direct_instance_list.add_placement_new();
}

void Renderable::loadTextures(bool generate_minimap,bool release_image)
{
	for(int i=0;i<textures.getLength();++i)
		textures[i].load(i,0,generate_minimap,release_image);
}

void Renderable::useTextures()
{
	for(int i=0;i<textures.getLength();++i)
		textures[i].use(i);
}

void Renderable::pretransformVertex()
{
	size_t vertex_count=getVertexCount();
	for(size_t idx=0;idx<vertex_count;++idx)
	{
		Vector3f& position=*(Vector3f*)(vertex_data+idx*vertex_attribs->getVertexSize()+vertex_attribs->getVertexAttributeInfo(VERTEX_ATTRIB_NAME_POSITION).getOffset());
		position=vertex_pretransform*position;
	}
}

void Renderable::bufferVertexData()
{
	size_t vertex_count=getVertexCount();
	if((!vertex_data)||(!vertex_count)) //如果没有顶点数据则直接返回
		return;
	pretransformVertex();
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,vertex_count*vertex_attribs->getVertexSize(),vertex_data,data_strategy);
	if(element_data&&getElementCount())
	{
		glGenBuffers(1,&ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*getElementCount(),element_data,data_strategy);
	}
	vertex_attribs->load(vao);
}

void Renderable::setVertexAttributeShader(VertexAttribute* vertex_attribs,ShaderProgram* shader)
{
	if(vertex_attribs==nullptr&&shader==nullptr)
	{
		switch(draw_strategy)
		{
		case DrawStrategy::INSTANCED:
			this->vertex_attribs=&VertexAttribute::getDefaultPTIVertexAttributes();
			this->shader=&ShaderProgram::getPTIDefaultShaderProgram();
			break;
		case DrawStrategy::NORMAL:
			this->vertex_attribs=&VertexAttribute::getDefaultPTVertexAttributes();
			this->shader=&ShaderProgram::getPTDefaultShaderProgram();
			break;
		}
	}else{
		this->vertex_attribs=vertex_attribs;
		this->shader=shader;
	}
}
