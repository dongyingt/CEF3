#include "SimpleClient.h"
#include  <tchar.h>

CSimpleClient::CSimpleClient()
{
}


CSimpleClient::~CSimpleClient()
{
}

void CSimpleClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	m_cefBrowser = browser;
}

bool CSimpleClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
	CefProcessId source_process,
	CefRefPtr<CefProcessMessage> message)
{
	const std::string& messageName = message->GetName();
	if (messageName == "login_msg")
	{
		// extract message
		CefRefPtr<CefListValue> args = message->GetArgumentList();
		CefString strUser = args->GetString(0);
		CefString strPassword = args->GetString(1);
		TCHAR szLog[256] = { 0 };
		_stprintf_s(szLog, 256, L"BrowserProcess, user - %s, password - %s\r\n", strUser.c_str(), strPassword.c_str());
		OutputDebugString(szLog);

		//send reply to render process
		CefRefPtr<CefProcessMessage> outMsg = CefProcessMessage::Create("login_reply");

		// Retrieve the argument list object.
		CefRefPtr<CefListValue> replyArgs = outMsg->GetArgumentList();

		// Populate the argument values.
		replyArgs->SetSize(1);
		replyArgs->SetInt(0, 0);

		// Send the process message to the renderer process.
		browser->SendProcessMessage(PID_RENDERER, outMsg);

		return true;
	}
	return false;
}

bool CSimpleClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	WindowOpenDisposition target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	switch (target_disposition)
	{
	case WOD_NEW_FOREGROUND_TAB:
	case WOD_NEW_BACKGROUND_TAB:
	case WOD_NEW_POPUP:
	case WOD_NEW_WINDOW:
		browser->GetMainFrame()->LoadURL(target_url);
		return true; //cancel create
	}

	return false;
}