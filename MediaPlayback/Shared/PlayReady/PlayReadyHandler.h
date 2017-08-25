#pragma once

#pragma once

#include <windows.media.h>
#include <Windows.Media.Playback.h>
#include <windows.media.protection.playready.h>
#include <string>
#include <mfidl.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;
using namespace ABI::Windows::Media::Protection;
using namespace ABI::Windows::Media::Protection::PlayReady;

typedef void( *DRMLicenseRequestedCallbackInternal)(void* objectThis, Microsoft::WRL::Wrappers::HString& licenseUriResult, Microsoft::WRL::Wrappers::HString& licenseCustomChallendgeDataResult);


class PlayReadyHandler
{
public:
	PlayReadyHandler(void* objectThis, DRMLicenseRequestedCallbackInternal fnCallback) : 
				  m_licenseCallback(fnCallback)
				, m_objectThis(objectThis)
	{
		assert(fnCallback);
		assert(objectThis);
	}

	static HRESULT AddStringProperty(ABI::Windows::Foundation::Collections::IMap<HSTRING, IInspectable*>* pIMap, PCWSTR key, PCWSTR value)
	{
		ComPtr<IActivationFactory> spGenericActivationFactoryInterface;
		ComPtr<ABI::Windows::Foundation::IPropertyValueStatics> pFactory;

		IFR(Windows::Foundation::GetActivationFactory(Wrappers::HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(), &spGenericActivationFactoryInterface));
		IFR(spGenericActivationFactoryInterface.As(&pFactory));

		ComPtr<ABI::Windows::Foundation::IPropertyValue> spValue;
		IFR(pFactory->CreateString(Wrappers::HStringReference(value).Get(), &spValue));

		boolean replaced;
		return pIMap->Insert(Wrappers::HStringReference(key).Get(),
			spValue.Get(), &replaced);
	}

	HRESULT InitalizeProtectionManager();

	ComPtr<IMediaProtectionManager>& GetProtectionManager()
	{
		if (m_spProtectionManager == nullptr)
			InitalizeProtectionManager();

		return m_spProtectionManager;
	}

private:
	HRESULT OnProtectionManager_ServiceRequested(
		IMediaProtectionManager* sender,
		IServiceRequestedEventArgs* srEvent);

	HRESULT OnProtectionManager_ComponentLoadFailed(
		IMediaProtectionManager* sender,
		IComponentLoadFailedEventArgs* e);

	HRESULT HandleIndividualizationRequest(ComPtr<IServiceRequestedEventArgs> spEvents);
	HRESULT HandleLicensingRequest(ComPtr<ABI::Windows::Media::Protection::PlayReady::IPlayReadyServiceRequest> spPRRequest, ComPtr<IServiceRequestedEventArgs> spEvents);
	HRESULT HandleServiceRequest(IServiceRequestedEventArgs* pIevtargs);
	HRESULT LicenseRequestCompletionCallback(ComPtr<IServiceRequestedEventArgs> spEvents, ABI::Windows::Foundation::IAsyncAction* info, ABI::Windows::Foundation::AsyncStatus status);

private:
	ComPtr<IMediaProtectionManager> m_spProtectionManager;
	EventRegistrationToken m_serviceRequestToken;
	EventRegistrationToken m_componentLoadFailedToken;
	DRMLicenseRequestedCallbackInternal m_licenseCallback;
	void* m_objectThis;
};