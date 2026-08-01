#include "Player/MyPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/MyEnhancedInputComponent.h"
#include "UI/MyHUD.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(MyInputContext)

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MyInputContext, 0);
	}
	
	bShowMouseCursor = false;
	const FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UMyEnhancedInputComponent* MyEnhancedInputComponent = CastChecked<UMyEnhancedInputComponent>(InputComponent);
	check(MoveInputAction)
	check(LookInputAction)
	check(ZoomInputAction)
	check(ToggleMenuInputAction)
	check(EscapeInputAction)
	MyEnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
	MyEnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
	MyEnhancedInputComponent->BindAction(ZoomInputAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Zoom);
	MyEnhancedInputComponent->BindAction(ToggleMenuInputAction, ETriggerEvent::Started, this, &AMyPlayerController::ToggleMenu);
	MyEnhancedInputComponent->BindAction(EscapeInputAction, ETriggerEvent::Started, this, &AMyPlayerController::EscapeCallback);
	
	MyEnhancedInputComponent->BindAbilityActions(InputConfig, this, &AMyPlayerController::AbilityInputPressed, &AMyPlayerController::AbilityInputReleased, &AMyPlayerController::AbilityInputHeld);
}

void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementValue = InputActionValue.Get<FVector2D>();
	
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	ControlledPawn->AddMovementInput(ForwardDirection, MovementValue.Y);
	ControlledPawn->AddMovementInput(RightDirection, MovementValue.X);
}

void AMyPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void AMyPlayerController::Zoom(const FInputActionValue& InputActionValue)
{
	const float ZoomValue = InputActionValue.Get<float>();
	
	const APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;
	
	USpringArmComponent* SpringArmComponent = ControlledPawn->FindComponentByClass<USpringArmComponent>();
	if (!SpringArmComponent) return;
	
	const float NewLength = SpringArmComponent->TargetArmLength - (ZoomValue * ZoomSpeed);
	SpringArmComponent->TargetArmLength = FMath::Clamp(NewLength, 200.f, 1000.f);
}

void AMyPlayerController::ToggleMenu(const FInputActionValue& InputActionValue)
{
	const AMyHUD* MyCustomHUD = Cast<AMyHUD>(GetHUD());
	if (!MyCustomHUD) return;
	MyCustomHUD->ToggleMenu();
}

void AMyPlayerController::EscapeCallback(const FInputActionValue& InputActionValue)
{
	const AMyHUD* MyCustomHUD = Cast<AMyHUD>(GetHUD());
	if (!MyCustomHUD) return;
	
	if (MyCustomHUD->CloseMenu()) return;
	if (MyCustomHUD->CloseSaveGameUI()) return;
	MyCustomHUD->OpenSaveGameUI();
}

void AMyPlayerController::AbilityInputPressed(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	GetMyAbilitySystemComponent()->AbilityInputPressed(InputTag);
}

void AMyPlayerController::AbilityInputReleased(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	GetMyAbilitySystemComponent()->AbilityInputReleased(InputTag);
}

void AMyPlayerController::AbilityInputHeld(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	GetMyAbilitySystemComponent()->AbilityInputHeld(InputTag);
}

UMyAbilitySystemComponent* AMyPlayerController::GetMyAbilitySystemComponent()
{
	// Input may trigger frequently, so only cast for first time
	if (MyASC == nullptr)
	{
		MyASC = Cast<UMyAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return MyASC;
}
