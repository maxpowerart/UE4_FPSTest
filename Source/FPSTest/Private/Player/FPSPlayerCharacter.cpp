#include "Player/FPSPlayerCharacter.h"

#include "FPSTest.h"
#include "Camera/CameraComponent.h"
#include "Character/FPSAbilitySystemComponent.h"
#include "Equipment/FPSEquipmentComponent.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/FPSInventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFPSPlayerCharacter::AFPSPlayerCharacter()
{
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	FirstPersonMesh->bReceivesDecals = false;
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetVisibility(false, true);

	Equipment = CreateDefaultSubobject<UFPSEquipmentComponent>("Equipment");
}

void AFPSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Equipment->LinkInventory(Cast<UFPSInventoryComponent>(GetPlayerState()->GetComponentByClass(UFPSInventoryComponent::StaticClass())));

	ASC->AbilityActorInfo->SkeletalMeshComponent = FirstPersonMesh;
}

void AFPSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AFPSPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AFPSPlayerCharacter::Turn);

	/**Bind ASC*/
	ASC->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("Confirm"),
			FString("Cancel"), FString("EFPSAbilityInputID"),
			static_cast<int32>(EFPSAbilityInputID::Confirm), static_cast<int32>(EFPSAbilityInputID::Cancel)));
}

void AFPSPlayerCharacter::MoveForward(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void AFPSPlayerCharacter::MoveRight(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void AFPSPlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AFPSPlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}
