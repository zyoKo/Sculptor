#pragma once

namespace Sculptor
{
	/*!
	 * \brief All the model path saved here, change DEFAULT_MESH in the bottom to acquire the default mesh file-path
	 */
	constexpr std::string_view CUBE{ "./Assets/Models/Cube/cube.obj" };

	constexpr std::string_view VIKING_ROOM{ "./Assets/Models/VikingRoom/viking-room.obj" };

	constexpr std::string_view BACKPACK_MODEL{ "./Assets/Models/Backpack/backpack.obj" };

	/*!
	 * \brief All the model path saved here, change DEFAULT_TEXTURE in the bottom to acquire the default texture file-path
	 */
	constexpr std::string_view BACKPACK_DIFFUSE_TEXTURE{ "./Assets/Models/Backpack/Textures/backpack-diffuse.jpg" };

	constexpr std::string_view VIKING_ROOM_DIFFUSE_TEXTURE{ "./Assets/Models/VikingRoom/Textures/viking-room.png" };

	// DEFAULT MESH, TEXTURES, etc.
	constexpr std::string_view DEFAULT_MESH = CUBE;

	constexpr std::string_view DEFAULT_TEXTURE = BACKPACK_DIFFUSE_TEXTURE;
}