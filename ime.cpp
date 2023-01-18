#include "BaseIME.h"
#include "IMM.h"
#include "JSC_Bridge_CPPTriggerEvent.h"

IngameIME::IMM g_ingameIME{};

void CALLBACK onCandidateList(libtf::CandidateList* list) {
	//spdlog::debug("onCandidateList");
	std::vector<std::wstring> vlist{ list->m_pCandidates.get(), list->m_pCandidates.get() + list->m_lPageSize };
	JSBridgeTriggerEvent("IME_OnCandidateList", vlist);
}

void CALLBACK onComposition(libtf::CompositionEventArgs* args) {
	//spdlog::debug("onComposition");
	JSBridgeTriggerEvent("IME_OnComposition",
		(int)args->m_state,
		args->m_state == libtf::CompositionState::Commit ? args->m_strCommit : args->m_strComposition,
		(int32_t)args->m_lCaretPos);
}

/*void CALLBACK onGetTextExt(PRECT prect) {
	// should return a bounding box of the composition string
}*/

void CALLBACK onAlphaMode(BOOL isAlphaMode) {
	// notify if ime in Alphanumeric input mode
	JSBridgeTriggerEvent("IME_OnAlphaMode", (bool)isAlphaMode);
}

void IME_Init(void* hwnd)
{
	g_ingameIME.Initialize(reinterpret_cast<HWND>(hwnd));
	g_ingameIME.m_sigAlphaMode = onAlphaMode;
	g_ingameIME.m_sigComposition = onComposition;
	g_ingameIME.m_sigCandidateList = onCandidateList;
	//g_ingameIME.m_sigGetTextExt = onGetTextExt;
}
