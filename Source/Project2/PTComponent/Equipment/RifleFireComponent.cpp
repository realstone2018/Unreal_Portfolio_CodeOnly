#include "PTComponent/Equipment//RifleFireComponent.h"
#include "Physics/PTCollision.h"

#define ENABLE_DRAW_DEBUG 0

void URifleFireComponent::FireProcess(FVector SpawnPoint, FRotator ShotDirection, float Range, int32 Damage)
{
	FHitResult HitResult;
	FVector DamageDirection = -ShotDirection.Vector();
	FVector End = SpawnPoint + ShotDirection.Vector() * Range;
	uint8 IsHit = FireLineTracing(HitResult, SpawnPoint, End);
	if (!IsHit)
	{
		return;
	}

	Gun->DamageToHitResult(HitResult, Damage, DamageDirection);
	OnHitTracing.Execute(HitResult, DamageDirection);
	
#if ENABLE_DRAW_DEBUG
	DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Red, true);
#endif
}

uint8 URifleFireComponent::FireLineTracing(FHitResult& HitResult, FVector SpawnPoint, FVector End)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(Gun->GetOwner());

#if ENABLE_DRAW_DEBUG
	DrawDebugLine(GetWorld(), OutLocation, End, FColor::Red, true, 5.f);
#endif
	
	return GetWorld()->LineTraceSingleByChannel(HitResult, SpawnPoint, End, CCHANNEL_PTBULLET, Params);
}