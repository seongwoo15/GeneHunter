// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Attack::UBTTask_Attack()
{
    NodeName = TEXT("Attack");
    bNotifyTaskFinished = true;
    bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if(OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    EnemyCharacter = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if(EnemyCharacter == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    EnemyCharacter->OnAttackEnded.AddUniqueDynamic(this, &UBTTask_Attack::OnAttackEnded);

    EnemyCharacter->MeleeAttack();
    CachedOwnerComp = &OwnerComp;

    return EBTNodeResult::InProgress;
}

void UBTTask_Attack::OnAttackEnded()
{
    if (EnemyCharacter)
    {
        // 델리게이트 바인딩 해제
        EnemyCharacter->OnAttackEnded.RemoveDynamic(this, &UBTTask_Attack::OnAttackEnded);
    }

    if (CachedOwnerComp)
    {
        // Task 완료
        FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
    }
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    if (EnemyCharacter)
    {
        EnemyCharacter->OnAttackEnded.RemoveDynamic(this, &UBTTask_Attack::OnAttackEnded);
    }

    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
