#pragma once
#include <map>
#include <string>

enum class DataType
{
	Int, Short, Long, Bool, Unknown
};


inline std::string DataTypeToString(DataType type) {
	static std::map<DataType, std::string> semStrings = {
		{DataType::Unknown, "Unknown"},
		{DataType::Int, "Int"},
		{DataType::Short, "Short"},
		{DataType::Long, "Long"},
		{DataType::Bool, "Bool"}
	};
	
	return semStrings.at(type);
}

inline DataType LexemeStringToDataType(const std::string& lexStr)
{
	static std::map<std::string, DataType> semStrings = {
		{"int", DataType::Int},
		{"short", DataType::Short},
		{"long", DataType::Long},
		{"bool", DataType::Bool},
	};
	return semStrings.at(lexStr);
}