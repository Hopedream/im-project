#pragma once

class PUBLIC_API CJPNotifyHandle
{
public:
	CJPNotifyHandle(void){}
	virtual ~CJPNotifyHandle(void){}
public:
	virtual void OnCommand(WPARAM wParam, LPARAM lParam){}
	virtual void OnTabSwitch(CDuiString name){}
	virtual void OnReturn(TNotifyUI& msg){}
	virtual void OnTextChanged(TNotifyUI& msg){}
	virtual void OnListItemClicked(TNotifyUI& msg){}
	virtual void OnListRClicked(TNotifyUI& msg){}
	virtual void OnItemSelecked(TNotifyUI& msg){}
	virtual void OnLinkClicked(TNotifyUI& msg){}
	virtual void OnOptionSelected(TNotifyUI& msg){}
	virtual void OnSetFocus(TNotifyUI& msg){}
	virtual void OnKillFocus(TNotifyUI& msg){}
};