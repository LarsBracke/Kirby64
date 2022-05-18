#pragma once

class KirbyController : public ControllerComponent
{
public:
	KirbyController(const PxCapsuleControllerDesc& controllerDesc);
	~KirbyController() override = default;

	KirbyController(const KirbyController& other) = delete;
	KirbyController(KirbyController&& other) noexcept = delete;
	KirbyController& operator=(const KirbyController& other) = delete;
	KirbyController& operator=(KirbyController&& other) noexcept = delete;

protected:

private:
};

