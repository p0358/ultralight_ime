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
#include <stdexcept>
#include "JSC_Bridge_TypeBinder.h"

ultralight::RefPtr<ultralight::JSContext> JSCtx();

JSObjectRef JSC_GetFunctionByPropertyNameFromObject(JSContextRef ctx, JSObjectRef parentObject, JSStringRef propertyName) {
	JSValueRef result = JSObjectGetProperty(ctx, parentObject, propertyName, nullptr);
	if (!JSValueIsObject(ctx, result)) [[unlikely]] return nullptr;
	JSObjectRef obj = JSValueToObject(ctx, result, nullptr);
	if (!JSObjectIsFunction(ctx, obj)) [[unlikely]] return nullptr;
	return obj;
}

class JSC_EventBinder
{
	ultralight::RefPtr<ultralight::JSContext> jsctx;
	JSObjectRef globalObject = nullptr;
	JSObjectRef callbackFunction = nullptr;
	JSValueRef* args = nullptr;
	int currentArg = 0;
	int argCount = 0;

	JSValueRef* GetCurrentArg()
	{
		if (currentArg == argCount)
			throw std::runtime_error("[JSC_EventBinder] Too many arguments");
		return &args[++currentArg];
	}

public:
	JSC_EventBinder() : jsctx(JSCtx()) { }
	~JSC_EventBinder() { if (args) delete args; }

	void BeginEvent(const char* name, int argCount)
	{
		this->argCount = argCount;
		args = new JSValueRef[argCount + 1];

		globalObject = JSContextGetGlobalObject(jsctx->ctx());
		JSStringRef _nativeEventCallbackString = JSStringCreateWithUTF8CString("_nativeEventCallback");
		callbackFunction = JSC_GetFunctionByPropertyNameFromObject(jsctx->ctx(), globalObject, _nativeEventCallbackString);
		JSStringRelease(_nativeEventCallbackString);
		if (!callbackFunction) [[unlikely]]
		{
			throw std::runtime_error("[JSC_EventBinder::BeginEvent] _nativeEventCallback is not registered");
			return;
		}

		auto eventNameString = JSStringCreateWithUTF8CString(name);
		args[0] = JSValueMakeString(jsctx->ctx(), eventNameString);
		JSStringRelease(eventNameString);
	}

	void EndEvent()
	{
		if (!callbackFunction || args == nullptr) [[unlikely]]
		{
			throw std::runtime_error("[EventBinder::EndEvent] Fault");
			return;
		}
		JSObjectCallAsFunction(jsctx->ctx(), callbackFunction, globalObject, argCount + 1, args, nullptr);
	}

	template <typename T> void BindArgument(T in) { *GetCurrentArg() = JSC_TypeBinder::Bind(jsctx->ctx(), in); }

	static void TriggerEvent(const char* name)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 0);
		binder.EndEvent();
	}
	template <typename T1>
	static void TriggerEvent(const char* name, const T1& argument1)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 1);
		binder.BindArgument(argument1);
		binder.EndEvent();
	}
	template <typename T1, typename T2>
	static void TriggerEvent(const char* name, const T1& argument1, const T2& argument2)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 2);
		binder.BindArgument(argument1);
		binder.BindArgument(argument2);
		binder.EndEvent();
	}
	template <typename T1, typename T2, typename T3>
	static void TriggerEvent(const char* name, const T1& argument1, const T2& argument2, const T3& argument3)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 3);
		binder.BindArgument(argument1);
		binder.BindArgument(argument2);
		binder.BindArgument(argument3);
		binder.EndEvent();
	}
	template <typename T1, typename T2, typename T3, typename T4>
	static void TriggerEvent(const char* name, const T1& argument1, const T2& argument2, const T3& argument3, const T4& argument4)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 4);
		binder.BindArgument(argument1);
		binder.BindArgument(argument2);
		binder.BindArgument(argument3);
		binder.BindArgument(argument4);
		binder.EndEvent();
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	static void TriggerEvent(const char* name, const T1& argument1, const T2& argument2, const T3& argument3, const T4& argument4, const T5& argument5)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 5);
		binder.BindArgument(argument1);
		binder.BindArgument(argument2);
		binder.BindArgument(argument3);
		binder.BindArgument(argument4);
		binder.BindArgument(argument5);
		binder.EndEvent();
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	static void TriggerEvent(const char* name, const T1& argument1, const T2& argument2, const T3& argument3, const T4& argument4, const T5& argument5, const T6& argument6)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 6);
		binder.BindArgument(argument1);
		binder.BindArgument(argument2);
		binder.BindArgument(argument3);
		binder.BindArgument(argument4);
		binder.BindArgument(argument5);
		binder.BindArgument(argument6);
		binder.EndEvent();
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	static void TriggerEvent(const char* name, const T1& argument1, const T2& argument2, const T3& argument3, const T4& argument4, const T5& argument5, const T6& argument6, const T7& argument7)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 7);
		binder.BindArgument(argument1);
		binder.BindArgument(argument2);
		binder.BindArgument(argument3);
		binder.BindArgument(argument4);
		binder.BindArgument(argument5);
		binder.BindArgument(argument6);
		binder.BindArgument(argument7);
		binder.EndEvent();
	}
	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	static void TriggerEvent(const char* name, const T1& argument1, const T2& argument2, const T3& argument3, const T4& argument4, const T5& argument5, const T6& argument6, const T7& argument7, const T8& argument8)
	{
		JSC_EventBinder binder{};
		binder.BeginEvent(name, 8);
		binder.BindArgument(argument1);
		binder.BindArgument(argument2);
		binder.BindArgument(argument3);
		binder.BindArgument(argument4);
		binder.BindArgument(argument5);
		binder.BindArgument(argument6);
		binder.BindArgument(argument7);
		binder.BindArgument(argument8);
		binder.EndEvent();
	}
};

template<typename... Args>
void JSBridgeTriggerEvent(const char* name, Args... args)
{
	JSC_EventBinder::TriggerEvent(name, args...);
}
