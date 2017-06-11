#include "neon_texture.h"

texture::texture()
{
	Initialised = false;
}

texture::~texture()
{
	if(Initialised && Content != 0)
	{
		free(Content);
		Content = 0;
	}
}

// @NOTE: image row start with bottom left and end with top right.
void texture::LoadFromFile(char const *Filename)
{
	read_file_result Data = Platform->ReadFile(Filename);

	u8 *InBytes = (u8*)Data.Content;

	u8 TGAType = *(u8 *)(InBytes + 2);
	Assert(TGAType == 2); // @TODO: Error message 

	Width  = *(u16 *)(InBytes + 12);
	Height = *(u16 *)(InBytes + 14);

	u8 BitsPerPixel = *(u8 *)(InBytes + 16);
	Assert(BitsPerPixel == 32); // @TODO: Error message

	ContentSize = Width * Height * (32/8); // bits/pixel = 32

	Content = malloc(ContentSize);
	
	memcpy(Content, InBytes + sizeof(tga_header), ContentSize);
	
	Platform->FreeFileMemory(&Data);

	// Little-endian 
	// On disk : 		  BB GG RR AA
	// In Mem  : 		  AA RR GG BB (Disk BBGGRRAA)
	// After Processing : RR GG BB AA (Mem 0xAABBGGRR)
	u32 *PixelPointer = (u32 *)Content;
	for(u32 PixelOffset = 0; PixelOffset < (ContentSize / 4); ++PixelOffset)
	{
		u32 *Pixel = (PixelPointer + PixelOffset);
		u32 B = (*Pixel & 0x000000FF) << 16;
		u32 G = (*Pixel & 0x0000FF00) ;//<< 8;
		u32 R = (*Pixel & 0x00FF0000) >> 16;
		u32 A = (*Pixel & 0xFF000000);// >> 24;
		*Pixel = R | G | B | A;
	}
	
	Convention = BOTTOM_LEFT_ZERO;
	Initialised = true;
	
	// Test if the rows starts from top left or bottom left
	// @NOTE: The image descriptor bits are maybe incorrect in GIMP exported TGA file.
	//
	// u8 ImageDescriptor = *(u8 *)(InBytes + 17);
	// if((ImageDescriptor & 1<<5) == 0x20)
	// {
	// 	Result->VFliped = false;
	// }
	// else
	// {
	// 	Result->VFliped = true;
	// }
	//
	// if((ImageDescriptor & 1<<4) == 0x10)
	// {
	// 	Assert(!"Color within row start with the right side & end with left side");
	// } 
}

void texture::FlipVertically()
{
	Assert(Initialised);

	u32 *Data = (u32 *)malloc(ContentSize);

	memcpy(Data, Content, ContentSize);
	for(int Row = 0; Row < Height; ++Row)
	{
		for(int Col = 0; Col < Width; ++Col)
		{	
			*((u32 *)(Content) + ((Width) * (Height - 1 - Row)) + Col)
				= *(Data + (Width) * Row + Col); 
		}
	}

	free(Data);

	if(Convention == TOP_LEFT_ZERO)
	{
		Convention = BOTTOM_LEFT_ZERO;
	}
	else
	{
		Convention = TOP_LEFT_ZERO;
	}
}

void texture::FreeMemory()
{
	Assert(Initialised);

	if(Content != 0)
	{
		free(Content);
		Content = 0;
	}
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	Texture Utility functions
////
// @NOTE: Image row start with bottom left and end with top right.
// texture* LoadTextureFromFile(char const * Filename)
// {

// 	texture *Result = (texture *)malloc(sizeof(texture));
	
// 	read_file_result Data = Platform->ReadFile(Filename);

// 	u8 *InBytes = (u8*)Data.Content;

// 	u8 TGAType = *(u8 *)(InBytes + 2);
// 	Assert(TGAType == 2); // @TODO: Error message 

// 	Result->Width  = *(u16 *)(InBytes + 12);
// 	Result->Height = *(u16 *)(InBytes + 14);

// 	u8 BitsPerPixel = *(u8 *)(InBytes + 16);
// 	Assert(BitsPerPixel == 32); // @TODO: Error message
// 	Result->BitsPerPixel = 32;

// 	Result->ContentSize = Result->Width * Result->Height * (BitsPerPixel/8); 

// 	Result->Content = malloc(Result->ContentSize);
	
// 	memcpy(Result->Content, InBytes + sizeof(tga_header), Result->ContentSize);
	
// 	Platform->FreeFileMemory(&Data);

// 	// Little-endian 
// 	// On disk : 		  BB GG RR AA
// 	// In Mem  : 		  AA RR GG BB (Disk BBGGRRAA)
// 	// After Processing : RR GG BB AA (Mem 0xAABBGGRR)
// 	u32 *PixelPointer = (u32 *)Result->Content;
// 	for(u32 PixelOffset = 0; PixelOffset < (Result->ContentSize / 4); ++PixelOffset)
// 	{
// 		u32 *Pixel = (PixelPointer + PixelOffset);
// 		u32 B = (*Pixel & 0x000000FF) << 16;
// 		u32 G = (*Pixel & 0x0000FF00) ;//<< 8;
// 		u32 R = (*Pixel & 0x00FF0000) >> 16;
// 		u32 A = (*Pixel & 0xFF000000);// >> 24;
// 		*Pixel = R | G | B | A;
// 	}
	
// 	Result->Convention = BOTTOM_LEFT_ZERO;
	
// 	/// Test if the rows starts from top left or bottom left
// 	// @NOTE: The image descriptor bits are maybe incorrect in GIMP exported TGA file.
// 	//
// 	// u8 ImageDescriptor = *(u8 *)(InBytes + 17);
// 	// if((ImageDescriptor & 1<<5) == 0x20)
// 	// {
// 	// 	Result->VFliped = false;
// 	// }
// 	// else
// 	// {
// 	// 	Result->VFliped = true;
// 	// }

// 	// if((ImageDescriptor & 1<<4) == 0x10)
// 	// {
// 	// 	Assert(!"Color within row start with the right side & end with left side");
// 	// } 
	
// 	return Result;
// }

// void FlipTextureVertically(texture *Texture)
// {
// 	u32 *Data = (u32 *)malloc(Texture->ContentSize);

// 	memcpy(Data, Texture->Content, Texture->ContentSize);
// 	for(int Row = 0; Row < Texture->Height; ++Row)
// 	{
// 		for(int Col = 0; Col < Texture->Width; ++Col)
// 		{	
// 			*((u32 *)(Texture->Content) + ((Texture->Width) * (Texture->Height - 1 - Row)) + Col)
// 				= *(Data + (Texture->Width) * Row + Col); 
// 		}
// 	}

// 	free(Data);

// 	if(Texture->Convention == TOP_LEFT_ZERO)
// 		Texture->Convention = BOTTOM_LEFT_ZERO;
// 	else
// 		Texture->Convention = TOP_LEFT_ZERO;
// }

// void FreeTextureMemory(texture *Texture)
// {
// 	free(Texture->Content);
// }

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	Texture Debugging function
////
void DebugTextureSave_(char const * Filename, texture *Texture)
{	
	if(Texture->Convention == BOTTOM_LEFT_ZERO)
	{
		Texture->FlipVertically();
		// FlipTextureVertically(Texture);
	}

	tga_header Header = {};

	Header.IDLength = 0;       
	Header.ColorMapType = 0;   
	Header.ImageType = 2;      
	Header.CMapStart = 0;      
	Header.CMapLength = 0;     
	Header.CMapDepth = 0;      
	Header.XOffset = 0;        
	Header.YOffset = 0;        
	Header.Width = (u16)Texture->Width;          
	Header.Height = (u16)Texture->Height;         
	Header.PixelDepth = 32;     
	Header.ImageDescriptor = 0x20;
	
	// Change the colors byte position
	// Little-endian
	// Spec:   AA RR GG BB [ BB GG RR AA (On file) ]
	u32 *PixelPointer = (u32 *)Texture->Content;
	for(u32 PixelOffset = 0; PixelOffset < (Texture->ContentSize / 4); ++PixelOffset)
	{
		u32 *Pixel = (PixelPointer + PixelOffset);
	
		u32 R = (*Pixel & 0x000000FF) <<16;
		u32 G = (*Pixel & 0x0000FF00);
		u32 B = (*Pixel & 0x00FF0000) >> 16;
		u32 A = (*Pixel & 0xFF000000);
		*Pixel = A | R | G | B;
	}

	void *FileContent = malloc(sizeof(tga_header) + Texture->ContentSize);
	memcpy(FileContent, &Header, sizeof(tga_header));
	memcpy((u8 *)FileContent + sizeof(tga_header), Texture->Content, Texture->ContentSize);
	Platform->WriteFile(Filename, sizeof(tga_header) + Texture->ContentSize, FileContent);

	free(FileContent);
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
////
////	TextureAtlas functions
////
texture_atlas::texture_atlas()
{
	Initialised = false;
}

texture_atlas::~texture_atlas()
{
	if(Initialised && Content != 0)
	{
		free(Content);
		Content = 0;
	}
}

void texture_atlas::TextureAtlas(u32 AtlasWidth, u32 AtlasHeigth, u16 AtlasPadding)
{
	Width = AtlasWidth;
	Height = AtlasHeigth;
	Padding = AtlasPadding;

	Content = malloc(Width * Height * 4);
	if(Content == 0)
	{
		Assert(!"Error");
	}
	
	ContentSize = Width * Height * 4;

	memset(Content, 0, ContentSize);

	Node.Child[0] = 0;
	Node.Child[1] = 0;
	Node.Rect.OriginX = 1;
	Node.Rect.OriginY = 1;
	Node.Rect.Width = Width;
	Node.Rect.Height = Height;
	Node.Filled = false;
	Initialised = true;

#if 1
	// make all pixels pink for debugging 
	u32 *Pixel = (u32 *)Content;	
	for(u32 x = 0; x < Width * Height; ++x)
	{
		*Pixel++ = 0xFFFF00FF; 
	}
#endif
}

static binary_t_node* Atlas_Insert(binary_t_node *Node, texture *Texture, u32 Padding)
{
	// if we're not a leaf node
	if(Node->Child[0] != 0 && Node->Child[1] != 0)
	{
		binary_t_node *NewNode;

		// try inserting into first child
		NewNode = Atlas_Insert(Node->Child[0], Texture, Padding);
		
		// if new node is not null
		if(NewNode != 0)
		{
			return NewNode;
		}
		else
		{
			// no room in first child, try in second child
			return Atlas_Insert(Node->Child[1], Texture, Padding);
		}
	}
	// if we're a leaf node
	else
	{
		// if there's already a texture here, or the we're too small for the texture
		if((Node->Filled) || (Node->Rect.Width < Texture->Width + Padding) || 
			(Node->Rect.Height < Texture->Height + Padding))
		{

			return 0;
		}

		// if we're just right size
		if((Node->Rect.Width == Texture->Width + Padding) && 
			(Node->Rect.Height == Texture->Height + Padding))
		{
			Node->Filled = true;

			Platform->Log(INFO, "Node O_x=%d O_y=%d W=%d H=%d\n", Node->Rect.OriginX,Node->Rect.OriginY,
														Node->Rect.Width, Node->Rect.Height);

			return Node;
		}

		Node->Child[0] = (binary_t_node *)malloc(sizeof(binary_t_node));
		Node->Child[1] = (binary_t_node *)malloc(sizeof(binary_t_node));
		
		Node->Child[0]->Filled = false;
		Node->Child[1]->Filled = false;

		Node->Child[0]->Child[0] = 0;
		Node->Child[0]->Child[1] = 0;
		Node->Child[1]->Child[0] = 0;
		Node->Child[1]->Child[1] = 0;


		// decide which way to split
		u32 dw = Node->Rect.Width  - (Texture->Width + Padding);
		u32 dh = Node->Rect.Height - (Texture->Height + Padding);

		if(dw > dh)
		{
			// divide vertically
			// needs testing
			Node->Child[0]->Rect.OriginX = Node->Rect.OriginX;
			Node->Child[0]->Rect.OriginY = Node->Rect.OriginY;
			Node->Child[0]->Rect.Width	 = (Texture->Width + Padding);
			Node->Child[0]->Rect.Height  = Node->Rect.Height;
			
			Node->Child[1]->Rect.OriginX = Node->Rect.OriginX + (Texture->Width + Padding);
			Node->Child[1]->Rect.OriginY = Node->Rect.OriginY;
			Node->Child[1]->Rect.Width	 = Node->Rect.Width - (Texture->Width + Padding);
			Node->Child[1]->Rect.Height  = Node->Rect.Height;

			return Atlas_Insert(Node->Child[0], Texture, Padding);
		}
		else
		{
			// divide horizontally
			// needs testing
			Node->Child[0]->Rect.OriginX = Node->Rect.OriginX;
			Node->Child[0]->Rect.OriginY = Node->Rect.OriginY;
			Node->Child[0]->Rect.Width	 = (Texture->Width + Padding);
			Node->Child[0]->Rect.Height  = (Texture->Height + Padding);
			
			Node->Child[1]->Rect.OriginX = Node->Rect.OriginX;
			Node->Child[1]->Rect.OriginY = Node->Rect.OriginY + (Texture->Height + Padding);
			Node->Child[1]->Rect.Width	 = Node->Rect.Width;
			Node->Child[1]->Rect.Height  = Node->Rect.Height - (Texture->Height + Padding);

			return Atlas_Insert(Node->Child[0], Texture, Padding);
		}
	}
}

texture_coordinates texture_atlas::PackTexture(texture *Texture)
{
	Assert(Initialised == true);

	if(Texture->Convention == BOTTOM_LEFT_ZERO)
	{
		Texture->FlipVertically();
	}

	binary_t_node *NodeSlot = Atlas_Insert(&Node, Texture, Padding);

	if(NodeSlot == 0)
	{
		Assert(!"Null returned.");
	}

	texture_coordinates TCoord;

	TCoord.BL_X = ((r32)NodeSlot->Rect.OriginX - 1) / (Width - 1);
	TCoord.BL_Y = ((r32)NodeSlot->Rect.OriginY + NodeSlot->Rect.Height - 3) / (Height - 1);
	TCoord.TR_X = ((r32)NodeSlot->Rect.OriginX + NodeSlot->Rect.Width - 3) / (Width - 1);
	TCoord.TR_Y = ((r32)NodeSlot->Rect.OriginY - 1) / (Height - 1);

	// Platform->Log(INFO, "BL_x=%f BL_y=%f TR_x=%f TR_y=%f\n", TCoord.BL_X, TCoord.BL_Y, TCoord.TR_X, TCoord.TR_Y);

	// copy the texture on the atlas at its position
	// 	x and y in range [1, width or height]
	//	to be written in range [0, Width or height - 1]
	//	write_at(x, y) = (u32)content_ptr + (x-1) + ((y-1) * width)
	u32 *pTextureContent = (u32 *)Texture->Content;
	
	for(u32 y = NodeSlot->Rect.OriginY; y < NodeSlot->Rect.OriginY + NodeSlot->Rect.Height - Padding; ++y)
	{
		for(u32 x = NodeSlot->Rect.OriginX; x < NodeSlot->Rect.OriginX + NodeSlot->Rect.Width - Padding; ++x)
		{
			u32 *Pixel = (u32 *)Content + (x - 1) + ((y-1) * Width);
			*Pixel = *(pTextureContent);
			pTextureContent++;
		}
	}

	
	// Debugging purpose
	texture AtlasDebugCopy;
	AtlasDebugCopy.Width = Width;
	AtlasDebugCopy.Height = Height;
	AtlasDebugCopy.ContentSize = ContentSize;
	AtlasDebugCopy.Content = malloc(ContentSize);
	memcpy(AtlasDebugCopy.Content, Content, ContentSize);
	AtlasDebugCopy.Convention = TOP_LEFT_ZERO;
	DebugTextureSave("FontAtlas.tga", &AtlasDebugCopy);

	//Return
	return TCoord;
}

void texture_atlas::FreeMemory()
{
	Assert(Initialised);
	if(Content != 0)
	{
		free(Content);
		Content = 0;
	}
}

// void InitTextureAtlas(texture_atlas *Atlas, u32 Width, u32 Height, u16 Padding)
// {
// 	Atlas->Content = malloc(Width * Height * 4);
// 	if(Atlas->Content == 0)
// 	{
// 		Assert(!"Error");
// 	}

// 	Atlas->Width = Width;
// 	Atlas->Height = Height;
// 	Atlas->ContentSize = Width * Height * 4;
// 	Atlas->Padding = Padding;

// 	memset(Atlas->Content, 0, Atlas->ContentSize);

// 	Atlas->Node.Child[0] = 0;
// 	Atlas->Node.Child[1] = 0;
// 	Atlas->Node.Rect.OriginX = 1;
// 	Atlas->Node.Rect.OriginY = 1;
// 	Atlas->Node.Rect.Width = Width;
// 	Atlas->Node.Rect.Height = Height;
// 	Atlas->Node.Filled = false;
// 	Atlas->Initialised = true;

// #if 1
// 	// make all pixels pink for debugging 
// 	u32 *Pixel = (u32 *)Atlas->Content;	
// 	for(u32 x = 0; x < Atlas->Width * Atlas->Height; ++x)
// 	{

// 		*Pixel++ = 0xFFFF00FF; 
// 	}
// #endif
// }


// static binary_t_node* Atlas_Insert(binary_t_node *Node, texture *Texture, u32 Padding)
// {
// 	// if we're not a leaf node
// 	if(Node->Child[0] != 0 && Node->Child[1] != 0)
// 	{
// 		binary_t_node *NewNode;

// 		// try inserting into first child
// 		NewNode = Atlas_Insert(Node->Child[0], Texture, Padding);
		
// 		// if new node is not null
// 		if(NewNode != 0)
// 		{
// 			return NewNode;
// 		}
// 		else
// 		{
// 			// no room in first child, try in second child
// 			return Atlas_Insert(Node->Child[1], Texture, Padding);
// 		}
// 	}
// 	// if we're a leaf node
// 	else
// 	{
// 		// if there's already a texture here, or the we're too small for the texture
// 		if((Node->Filled) || (Node->Rect.Width < Texture->Width + Padding) || 
// 			(Node->Rect.Height < Texture->Height + Padding))
// 		{

// 			return 0;
// 		}

// 		// if we're just right size
// 		if((Node->Rect.Width == Texture->Width + Padding) && 
// 			(Node->Rect.Height == Texture->Height + Padding))
// 		{
// 			Node->Filled = true;

// 			Platform->Log(INFO, "Node O_x=%d O_y=%d W=%d H=%d\n", Node->Rect.OriginX,Node->Rect.OriginY,
// 														Node->Rect.Width, Node->Rect.Height);

// 			return Node;
// 		}

// 		Node->Child[0] = (binary_t_node *)malloc(sizeof(binary_t_node));
// 		Node->Child[1] = (binary_t_node *)malloc(sizeof(binary_t_node));
		
// 		Node->Child[0]->Filled = false;
// 		Node->Child[1]->Filled = false;

// 		Node->Child[0]->Child[0] = 0;
// 		Node->Child[0]->Child[1] = 0;
// 		Node->Child[1]->Child[0] = 0;
// 		Node->Child[1]->Child[1] = 0;


// 		// decide which way to split
// 		u32 dw = Node->Rect.Width  - (Texture->Width + Padding);
// 		u32 dh = Node->Rect.Height - (Texture->Height + Padding);

// 		if(dw > dh)
// 		{
// 			// divide vertically
// 			// needs testing
// 			Node->Child[0]->Rect.OriginX = Node->Rect.OriginX;
// 			Node->Child[0]->Rect.OriginY = Node->Rect.OriginY;
// 			Node->Child[0]->Rect.Width	 = (Texture->Width + Padding);
// 			Node->Child[0]->Rect.Height  = Node->Rect.Height;
			
// 			Node->Child[1]->Rect.OriginX = Node->Rect.OriginX + (Texture->Width + Padding);
// 			Node->Child[1]->Rect.OriginY = Node->Rect.OriginY;
// 			Node->Child[1]->Rect.Width	 = Node->Rect.Width - (Texture->Width + Padding);
// 			Node->Child[1]->Rect.Height  = Node->Rect.Height;

// 			return Atlas_Insert(Node->Child[0], Texture, Padding);
// 		}
// 		else
// 		{
// 			// divide horizontally
// 			// needs testing
// 			Node->Child[0]->Rect.OriginX = Node->Rect.OriginX;
// 			Node->Child[0]->Rect.OriginY = Node->Rect.OriginY;
// 			Node->Child[0]->Rect.Width	 = (Texture->Width + Padding);
// 			Node->Child[0]->Rect.Height  = (Texture->Height + Padding);
			
// 			Node->Child[1]->Rect.OriginX = Node->Rect.OriginX;
// 			Node->Child[1]->Rect.OriginY = Node->Rect.OriginY + (Texture->Height + Padding);
// 			Node->Child[1]->Rect.Width	 = Node->Rect.Width;
// 			Node->Child[1]->Rect.Height  = Node->Rect.Height - (Texture->Height + Padding);

// 			return Atlas_Insert(Node->Child[0], Texture, Padding);
// 		}
// 	}
// }

// texture_coordinates PackTexture(texture_atlas *Atlas, texture *Texture)
// {
// 	Assert(Atlas->Initialised == true);

// 	if(Texture->Convention == BOTTOM_LEFT_ZERO)
// 	{
// 		FlipTextureVertically(Texture);
// 	}

// 	binary_t_node *NodeSlot = Atlas_Insert(&Atlas->Node, Texture, Atlas->Padding);

// 	if(NodeSlot == 0)
// 	{
// 		Assert(!"Null returned.");
// 	}

// 	texture_coordinates TCoord;
// 	// needs division by width and height
// 	TCoord.BL_X = ((r32)NodeSlot->Rect.OriginX - 1) / (Atlas->Width - 1);
// 	TCoord.BL_Y = ((r32)NodeSlot->Rect.OriginY + NodeSlot->Rect.Height - 3) / (Atlas->Height - 1);
// 	TCoord.TR_X = ((r32)NodeSlot->Rect.OriginX + NodeSlot->Rect.Width - 3) / (Atlas->Width - 1);
// 	TCoord.TR_Y = ((r32)NodeSlot->Rect.OriginY - 1) / (Atlas->Height - 1);

// 	// Platform->Log(INFO, "BL_x=%f BL_y=%f TR_x=%f TR_y=%f\n", TCoord.BL_X, TCoord.BL_Y, TCoord.TR_X, TCoord.TR_Y);

// 	// copy the texture on the atlas at its position
// 	// 	x and y in range [1, width or height]
// 	//	to be written in range [0, Width or height - 1]
// 	//	write_at(x, y) = (u32)content_ptr + (x-1) + ((y-1) * width)

// 	u32 *pTextureContent = (u32 *)Texture->Content;
	
// 	for(u32 y = NodeSlot->Rect.OriginY; y < NodeSlot->Rect.OriginY + NodeSlot->Rect.Height - Atlas->Padding; ++y)
// 	{
// 		for(u32 x = NodeSlot->Rect.OriginX; x < NodeSlot->Rect.OriginX + NodeSlot->Rect.Width - Atlas->Padding; ++x)
// 		{
// 			u32 *Pixel = (u32 *)Atlas->Content + (x - 1) + ((y-1) * Atlas->Width);
// 			*Pixel = *(pTextureContent);
// 			pTextureContent++;
// 		}
// 	}

	
// 	// Debugging purpose
// 	texture AtlasDebugCopy;
// 	AtlasDebugCopy.Width = Atlas->Width;
// 	AtlasDebugCopy.Height = Atlas->Height;
// 	AtlasDebugCopy.ContentSize = Atlas->ContentSize;
// 	AtlasDebugCopy.Content = malloc(Atlas->ContentSize);
// 	memcpy(AtlasDebugCopy.Content, Atlas->Content, Atlas->ContentSize);
// 	AtlasDebugCopy.Convention = TOP_LEFT_ZERO;
// 	AtlasDebugCopy.BitsPerPixel = 32;
// 	DebugTextureSave("FontAtlas.tga", &AtlasDebugCopy);

// 	//Return
// 	return TCoord;
// }

// void ConvertAtlasToTexture(texture_atlas *Atlas, texture *Texture)
// {
// 	Texture->Width = Atlas->Width;
// 	Texture->Height = Atlas->Height;
// 	Texture->ContentSize = Atlas->ContentSize;
// 	Texture->Content = malloc(Atlas->ContentSize);
// 	memcpy(Texture->Content, Atlas->Content, Atlas->ContentSize);
// 	Texture->Convention = TOP_LEFT_ZERO;
// 	Texture->BitsPerPixel = 32;
// }