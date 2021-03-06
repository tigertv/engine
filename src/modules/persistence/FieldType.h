/**
 * @file
 */

#pragma once

#include "core/Enum.h"
#include "core/String.h"

namespace persistence {

// don't change the order without changing the string mapping
enum class FieldType {
	STRING,
	TEXT,
	LONG,
	INT,
	PASSWORD,
	TIMESTAMP,
	BOOLEAN,
	SHORT,
	BYTE,
	DOUBLE,
	BLOB,
	MAX
};
static constexpr int MAX_FIELDTYPES = core::enumVal(FieldType::MAX);

extern FieldType toFieldType(const core::String& type);
extern const char* toFieldType(const FieldType type);

}
