/*
 * This file is part of TFORevive project. Source: https://github.com/TFORevive/tforevive_cpp
 * Copyright (C) 2022-2023 p0358
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Lesser Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <Ultralight/JavaScript.h>
#include <memory>
#include <vector>

inline void JSC_ReadStringToStdString(JSContextRef ctx, JSStringRef string, std::string& output)
{
	output.resize(JSStringGetMaximumUTF8CStringSize(string));
	size_t size = JSStringGetUTF8CString(string, (char*)output.c_str(), output.capacity());
	output.resize(std::max<size_t>(1, size) - 1); // -1 cause it includes null terminator
}

inline std::string JSC_ReadStringToStdString(JSContextRef ctx, JSStringRef string)
{
	std::string out;
	JSC_ReadStringToStdString(ctx, string, out);
	return std::move(out);
}

class JSC_TypeBinder
{
public:
	static JSValueRef Bind(JSContextRef ctx, const char* in)
	{
		auto str = JSStringCreateWithUTF8CString(in);
		auto ret = JSValueMakeString(ctx, str);
		JSStringRelease(str);
		return ret;
	}
	static JSValueRef Bind(JSContextRef ctx, char* in) { return Bind(ctx, const_cast<const char*>(in)); }
	static JSValueRef Bind(JSContextRef ctx, const std::string& in) { return Bind(ctx, in.data()); }
	static JSValueRef Bind(JSContextRef ctx, const std::string* in) { return in ? Bind(ctx, in->data()) : Bind(ctx, nullptr); }
	static JSValueRef Bind(JSContextRef ctx, const wchar_t* in)
	{
		auto str = JSStringCreateWithCharacters(in, wcslen(in));
		auto ret = JSValueMakeString(ctx, str);
		JSStringRelease(str);
		return ret;
	}
	static JSValueRef Bind(JSContextRef ctx, const std::wstring& in)
	{
		auto str = JSStringCreateWithCharacters(in.data(), in.length());
		auto ret = JSValueMakeString(ctx, str);
		JSStringRelease(str);
		return ret;
	}
	static JSValueRef Bind(JSContextRef ctx, const std::wstring* in) { return Bind(ctx, *in); }
	static JSValueRef Bind(JSContextRef ctx, std::wstring* in) { return Bind(ctx, *in); }
	static JSValueRef Bind(JSContextRef ctx, double in) { return JSValueMakeNumber(ctx, in); }
	static JSValueRef Bind(JSContextRef ctx, float in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, int64_t in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, uint64_t in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, int32_t in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, uint32_t in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, int16_t in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, uint16_t in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, int8_t in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, uint8_t in) { return JSValueMakeNumber(ctx, static_cast<double>(in)); }
	static JSValueRef Bind(JSContextRef ctx, bool in) { return JSValueMakeBoolean(ctx, !!in); }
	static JSValueRef Bind(JSContextRef ctx, nullptr_t) { return JSValueMakeNull(ctx); }
	template <typename T> static JSValueRef Bind(JSContextRef ctx, T* in) { return in ? Bind(*in) : Bind(nullptr); }
	template <typename T> static JSValueRef Bind(JSContextRef ctx, std::shared_ptr<T> in) { return in ? Bind(in.get()) : Bind(nullptr); }
	template <typename T, size_t size> static JSValueRef Bind(JSContextRef ctx, T(&in)[size])
	{
		JSValueRef args[size];
		for (int i = 0; i < size; i++)
			args[i] = Bind(ctx, in[i]);
		return JSObjectMakeArray(ctx, size, args, nullptr);
	}
	template <typename T> static JSValueRef Bind(JSContextRef ctx, std::vector<T> in)
	{
		auto size = in.size();
		JSValueRef* args = reinterpret_cast<JSValueRef*>(_alloca(size * sizeof(JSValueRef*)));
		for (int i = 0; i < size; i++)
			args[i] = Bind(ctx, in.at(i));
		return JSObjectMakeArray(ctx, size, args, nullptr);
	}

	static double GetDouble(JSContextRef ctx, JSValueRef in) { return JSValueToNumber(ctx, in, nullptr); }
	static float GetFloat(JSContextRef ctx, JSValueRef in) { return static_cast<float>(JSValueToNumber(ctx, in, nullptr)); }
	static int64_t GetInt64(JSContextRef ctx, JSValueRef in) { return static_cast<int64_t>(JSValueToNumber(ctx, in, nullptr)); }
	static uint64_t GetUint64(JSContextRef ctx, JSValueRef in) { return static_cast<uint64_t>(JSValueToNumber(ctx, in, nullptr)); }
	static int32_t GetInt32(JSContextRef ctx, JSValueRef in) { return static_cast<int32_t>(JSValueToNumber(ctx, in, nullptr)); }
	static uint32_t GetUInt32(JSContextRef ctx, JSValueRef in) { return static_cast<uint32_t>(JSValueToNumber(ctx, in, nullptr)); }
	static int16_t GetInt16(JSContextRef ctx, JSValueRef in) { return static_cast<int16_t>(JSValueToNumber(ctx, in, nullptr)); }
	static uint16_t GetUInt16(JSContextRef ctx, JSValueRef in) { return static_cast<uint16_t>(JSValueToNumber(ctx, in, nullptr)); }
	static int8_t GetInt8(JSContextRef ctx, JSValueRef in) { return static_cast<int8_t>(JSValueToNumber(ctx, in, nullptr)); }
	static uint8_t GetUInt8(JSContextRef ctx, JSValueRef in) { return static_cast<uint8_t>(JSValueToNumber(ctx, in, nullptr)); }
	static bool GetBool(JSContextRef ctx, JSValueRef in) { return JSValueToBoolean(ctx, in); }
	static std::string GetStdString(JSContextRef ctx, JSValueRef in)
	{
		JSStringRef string = JSValueToStringCopy(ctx, in, nullptr);
		auto str = JSC_ReadStringToStdString(ctx, string);
		JSStringRelease(string);
		return str;
	}

	static void Get(JSContextRef ctx, JSValueRef in, double* out) { *out = JSValueToNumber(ctx, in, nullptr); }
	static void Get(JSContextRef ctx, JSValueRef in, float* out) { *out = static_cast<float>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, int64_t* out) { *out = static_cast<int64_t>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, uint64_t* out) { *out = static_cast<uint64_t>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, int32_t* out) { *out = static_cast<int32_t>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, uint32_t* out) { *out = static_cast<uint32_t>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, int16_t* out) { *out = static_cast<int16_t>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, uint16_t* out) { *out = static_cast<uint16_t>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, int8_t* out) { *out = static_cast<int8_t>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, uint8_t* out) { *out = static_cast<uint8_t>(JSValueToNumber(ctx, in, nullptr)); }
	static void Get(JSContextRef ctx, JSValueRef in, bool* out) { *out = JSValueToBoolean(ctx, in); }
	static void Get(JSContextRef ctx, JSValueRef in, std::string& out)
	{
		JSStringRef string = JSValueToStringCopy(ctx, in, nullptr);
		JSC_ReadStringToStdString(ctx, string, out);
		JSStringRelease(string);
	}
	static void Get(JSContextRef ctx, JSValueRef in, const std::string& out) { Get(ctx, in, const_cast<std::string&>(out)); }
	static void Get(JSContextRef ctx, JSValueRef in, const std::string* out) { Get(ctx, in, *out); }
	static void Get(JSContextRef ctx, JSValueRef in, std::string* out) { Get(ctx, in, *out); }
};
