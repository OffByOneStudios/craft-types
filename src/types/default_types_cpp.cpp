#include "common.h"

#include "core.h"
#include "default_types_cpp.h"

// stdlib
CRAFT_TYPE_DEFINE(std::string)
{
	_.use<PParse>().singleton<FunctionalParse>( [](std::string s) { return instance<std::string>::make(s); });
	_.use<PStringer>().singleton<FunctionalStringer>( [](::craft::instance<std::string> _this) { return *_this; } );
}

//// GLM
//CRAFT_TYPE_DEFINE(glm::vec2)
//{
//	_.use<PParse>().singleton<FunctionalParse>(
//		[](std::string s)
//		{
//			std::vector<std::string> _parts;
//			stdext::split(s, ' ', std::back_inserter(_parts));
//			if (_parts.size() != 2) throw craft_error("Invalid parse");
//			return instance<glm::vec2>::make(std::stof(_parts[0]), std::stof(_parts[1]));
//		});
//	_.use<PStringer>().singleton<FunctionalStringer>(
//		[](::craft::instance<glm::vec2> _this)
//		{ 
//			glm::vec2& ac = *_this;
//			return fmt::format("glm::vec2({0},{1})", ac.x, ac.y);
//		});
//}
//
//CRAFT_TYPE_DEFINE(glm::vec3)
//{
//	_.use<PParse>().singleton<FunctionalParse>(
//		[](std::string s)
//		{
//			std::vector<std::string> _parts;
//			stdext::split(s, ' ', std::back_inserter(_parts));
//			if (_parts.size() != 3) throw craft_error("Invalid parse");
//			return instance<glm::vec3>::make(std::stof(_parts[0]), std::stof(_parts[1]), std::stof(_parts[2]));
//		});
//	_.use<PStringer>().singleton<FunctionalStringer>(
//		[](::craft::instance<glm::vec3> _this)
//		{
//			glm::vec3& ac = *_this;
//			return fmt::format("glm::vec3({0},{1},{2})", ac.x, ac.y, ac.z);
//		});
//}
//CRAFT_TYPE_DEFINE(glm::vec4)
//{
//	_.use<PParse>().singleton<FunctionalParse>(
//		[](std::string s)
//		{
//			std::vector<std::string> _parts;
//			stdext::split(s, ' ', std::back_inserter(_parts));
//			if (_parts.size() != 4) throw craft_error("Invalid parse");
//			return instance<glm::vec4>::make(std::stof(_parts[0]), std::stof(_parts[1]), std::stof(_parts[2]), std::stof(_parts[3]));
//		});
//	_.use<PStringer>().singleton<FunctionalStringer>(
//		[](::craft::instance<glm::vec4> _this)
//		{
//			glm::vec4& ac = *_this;
//			return fmt::format("glm::vec4({0},{1},{2},{3})", ac.x, ac.y, ac.z, ac.w);
//		});
//}
//
//CRAFT_TYPE_DEFINE(glm::uvec2)
//{
//	_.use<PParse>().singleton<FunctionalParse>(
//		[](std::string s)
//	{
//		std::vector<std::string> _parts;
//		stdext::split(s, ' ', std::back_inserter(_parts));
//		if (_parts.size() != 2) throw craft_error("Invalid parse");
//		return instance<glm::uvec2>::make(std::stoi(_parts[0]), std::stoi(_parts[1]));
//	});
//	_.use<PStringer>().singleton<FunctionalStringer>(
//		[](::craft::instance<glm::uvec2> _this)
//	{
//		glm::uvec2& ac = *_this;
//		return fmt::format("glm::uvec2({0},{1})", ac.x, ac.y);
//	});
//}
//
//CRAFT_TYPE_DEFINE(glm::uvec3)
//{
//	_.use<PParse>().singleton<FunctionalParse>(
//		[](std::string s)
//	{
//		std::vector<std::string> _parts;
//		stdext::split(s, ' ', std::back_inserter(_parts));
//		if (_parts.size() != 3) throw craft_error("Invalid parse");
//		return instance<glm::uvec3>::make(std::stoi(_parts[0]), std::stoi(_parts[1]), std::stoi(_parts[2]));
//	});
//	_.use<PStringer>().singleton<FunctionalStringer>(
//		[](::craft::instance<glm::uvec3> _this)
//	{
//		glm::uvec3& ac = *_this;
//		return fmt::format("glm::uvec3({0},{1},{2})", ac.x, ac.y, ac.z);
//	});
//}
//CRAFT_TYPE_DEFINE(glm::uvec4)
//{
//	_.use<PParse>().singleton<FunctionalParse>(
//		[](std::string s)
//	{
//		std::vector<std::string> _parts;
//		stdext::split(s, ' ', std::back_inserter(_parts));
//		if (_parts.size() != 4) throw craft_error("Invalid parse");
//		return instance<glm::uvec4>::make(std::stoi(_parts[0]), std::stoi(_parts[1]), std::stoi(_parts[2]), std::stoi(_parts[3]));
//	});
//	_.use<PStringer>().singleton<FunctionalStringer>(
//		[](::craft::instance<glm::uvec4> _this)
//	{
//		glm::uvec4& ac = *_this;
//		return fmt::format("glm::uvec4({0},{1},{2},{3})", ac.x, ac.y, ac.z, ac.w);
//	});
//}
//
//CRAFT_TYPE_DEFINE(glm::mat3)
//{
//	_.use<PStringer>().singleton<FunctionalStringer>(
//		[](::craft::instance<glm::mat3> _this)
//		{
//			glm::mat3& ac = *_this;
//			return fmt::format("glm::mat3([\n{0:010.3f}, {1:010.3f}, {2:010.3f},\n{3:010.3f}, {4:010.3f}, {5:010.3f}\n{6:010.3f}, {7:010.3f}, {8:010.3f}\n])",
//				ac[0][0], ac[0][1], ac[0][2],
//				ac[1][0], ac[1][1], ac[1][2],
//				ac[2][0], ac[2][1], ac[2][2]
//			);
//		});
//}
//CRAFT_TYPE_DEFINE(glm::mat4)
//{
//	_.use<PStringer>().singleton<FunctionalStringer>(
//		[](::craft::instance<glm::mat4> _this)
//		{
//			glm::mat4& ac = *_this;
//			return fmt::format("glm::mat4([\n{0:010.3f}, {1:010.3f}, {2:010.3f}, {3:010.3f},\n{4:010.3f}, {5:010.3f}, {6:010.3f}, {7:010.3f},\n{8:010.3f}, {9:010.3f}, {10:010.3f}, {11:010.3f},\n{12:010.3f}, {13:010.3f}, {14:010.3f}, {15:010.3f}\n])",
//				ac[0][0], ac[0][1], ac[0][2], ac[0][3],
//				ac[1][0], ac[1][1], ac[1][2], ac[1][3],
//				ac[2][0], ac[2][1], ac[2][2], ac[2][3],
//				ac[3][0], ac[3][1], ac[3][2], ac[3][3]
//			);
//		});
//}
