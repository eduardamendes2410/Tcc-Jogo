#include "EnemyABehaviorTree.h"
#include "CompositeNodes.h"
#include "ConditionNodes.h"
#include "ActionNodes.h"
#include <memory>


std::shared_ptr<BehaviorTree> EnemyABehaviorTree::CreateSimpleFleeBehaviorTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();

    // Raiz da árvore
    auto root = std::make_shared<SelectorNode>();

    // --- NÓ 1: FUGA QUANDO VIDA BAIXA ---
    auto lowHealthSequence = std::make_shared<SequenceNode>();
    lowHealthSequence->AddChild(std::make_shared<IsLowHealthNode>(3.0f)); // Vida < 3
    lowHealthSequence->AddChild(std::make_shared<IsPlayerNearNode>(200.0f)); // Player está por perto

    // Sub-sequência de fuga e cura
    auto fleeAndHeal = std::make_shared<SequenceNode>();
    fleeAndHeal->AddChild(std::make_shared<FleeActionNode>(300.0f)); // foge
    fleeAndHeal->AddChild(std::make_shared<HealOverTimeNode>(0.1f));

    lowHealthSequence->AddChild(fleeAndHeal);
    root->AddChild(lowHealthSequence);

    // --- ESTRATÉGIA: RECUAR PARA O LÍDER (Prioridade Máxima) ---
    auto retreatToLeader = std::make_shared<SequenceNode>(); 
    retreatToLeader->AddChild(std::make_shared<HasLeaderAndIsWeakNode>(5.0f));
    retreatToLeader->AddChild(std::make_shared<RepositionBehindLeaderNode>(50.0f)); // Ação: Mova-se para 50px atrás do líder
    retreatToLeader->AddChild(std::make_shared<AttackActionNode>());

    root->AddChild(retreatToLeader);

    // --- NÓ 2: COMPORTAMENTO NORMAL ---
    auto normalBehavior = std::make_shared<SelectorNode>();

    auto attackBehavior = std::make_shared<SequenceNode>();
    attackBehavior->AddChild(std::make_shared<IsPlayerNearNode>(100.0f));
    attackBehavior->AddChild(std::make_shared<AttackActionNode>());

    normalBehavior->AddChild(attackBehavior);

    normalBehavior->AddChild(std::make_shared<ChaseActionNode>()); // persegue o player
    normalBehavior->AddChild(std::make_shared<PatrolActionNode>(150.0f)); // ou patrulha

    root->AddChild(normalBehavior);

    behaviorTree->SetRoot(root);
    return behaviorTree;
}