#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UMotionWarpingComponent;
class UStateTreeComponent;
class UInputConfig;
class UMyAbilitySystemComponent;
struct FGameplayTag;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	AMyPlayerController();
	
	/** GenericTeamAgentInterface */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override { TeamID = NewTeamID; }
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
	/** end GenericTeamAgentInterface */

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	// 0 = Player, 1 = Enemy, 2 = Neutral
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup|AI")
	FGenericTeamId TeamID = 0;
	
private:
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Zoom(const FInputActionValue& InputActionValue);
	void ToggleMenu(const FInputActionValue& InputActionValue);
	void EscapeCallback(const FInputActionValue& InputActionValue);

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
	UMyAbilitySystemComponent* GetMyAbilitySystemComponent();
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Input")
	TObjectPtr<UInputMappingContext> MyInputContext;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Input")
	TObjectPtr<UInputAction> MoveInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Input")
	TObjectPtr<UInputAction> LookInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Input")
	TObjectPtr<UInputAction> ZoomInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Input")
	TObjectPtr<UInputAction> ToggleMenuInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Input")
	TObjectPtr<UInputAction> EscapeInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Input")
	float ZoomSpeed = 50.0f;
	
	UPROPERTY()
	TObjectPtr<UMyAbilitySystemComponent> MyASC;
	
	UPROPERTY(EditDefaultsOnly, Category="Setup|Input")
	TObjectPtr<UInputConfig> InputConfig;
};
