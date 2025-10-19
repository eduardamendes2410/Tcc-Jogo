//
// Created by AI Assistant for Behavior Tree implementation
// Behavior Tree especial para o Inimigo A com comportamentos inteligentes
//

#include "EnemyABehaviorTree.h"
#include "CompositeNodes.h"
#include "ConditionNodes.h"
#include "ActionNodes.h"
#include <memory>

//esse codigo ele vai implementar diferentes tipos de arvores de comportamento, entao eu posso escolher entre um inimigo
//mais cauteloso, ou um que ataca mais

std::shared_ptr<BehaviorTree> EnemyABehaviorTree::CreateSimpleFleeBehaviorTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();

    // Raiz da árvore
    auto root = std::make_shared<SelectorNode>();

    //SE QUALQUER UM DESSES FALHAR ELE VAI PASSAR PRO NO DE "COMPORTAMENTO NORMAL"
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


//
//
// std::shared_ptr<BehaviorTree> EnemyABehaviorTree::CreateIntelligentBehaviorTree() {
//     auto behaviorTree = std::make_shared<BehaviorTree>();
//
//     // Behavior Tree inteligente com múltiplas estratégias adaptativas
//     auto root = std::make_shared<SelectorNode>();
//
//     // Estratégia 1: Fuga inteligente quando com pouca vida
//     //como é um selectorNode ele vai tentar essa primeira estrategia, se der errado ele vai tentar a segunda ...
//     auto smartFlee = CreateSmartFleeSequence();
//     root->AddChild(smartFlee);
//
//     // Estratégia 2: Combate tático quando saudável
//     auto tacticalCombat = CreateTacticalCombatSequence();
//     root->AddChild(tacticalCombat);
//
//     // Estratégia 3: Patrulha adaptativa
//     auto adaptivePatrol = CreateAdaptivePatrolSequence();
//     root->AddChild(adaptivePatrol);
//
//     behaviorTree->SetRoot(root);
//     return behaviorTree;
// }
//
// std::shared_ptr<BehaviorTree> EnemyABehaviorTree::CreateTacticalBehaviorTree() {
//     auto behaviorTree = std::make_shared<BehaviorTree>();
//
//     // Behavior Tree com foco em táticas de fuga e sobrevivência
//     auto root = std::make_shared<SelectorNode>();
//
//     // Estratégia 1: Fuga desesperada quando muito ferido
//     //se a vida for igual ou menor que 1 e o jogador estiver perto ele vai tentar fugir rapido
//     auto desperateFlee = std::make_shared<SequenceNode>();
//     desperateFlee->AddChild(std::make_shared<IsLowHealthNode>(1.0f));
//     desperateFlee->AddChild(std::make_shared<IsPlayerNearNode>(300.0f));
//     desperateFlee->AddChild(std::make_shared<FleeActionNode>(400.0f)); // Foge MUITO mais rápido
//     root->AddChild(desperateFlee);
//
//     // Estratégia 2: Fuga preventiva quando moderadamente ferido
//     auto preventiveFlee = std::make_shared<SequenceNode>();
//     preventiveFlee->AddChild(std::make_shared<IsLowHealthNode>(2.0f));
//     preventiveFlee->AddChild(std::make_shared<IsPlayerNearNode>(250.0f));
//     preventiveFlee->AddChild(std::make_shared<FleeActionNode>(350.0f)); // Foge mais rápido
//     root->AddChild(preventiveFlee);
//
//     // Estratégia 3: Combate defensivo quando saudável
//     auto defensiveCombat = std::make_shared<SequenceNode>();
//     defensiveCombat->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
//     auto combatOrRetreat = std::make_shared<SelectorNode>();
//
//     // Sub-estratégia: Ataque à distância segura
//     auto safeAttack = std::make_shared<SequenceNode>();
//     safeAttack->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(100.0f));
//     safeAttack->AddChild(std::make_shared<CanShootNode>());
//     safeAttack->AddChild(std::make_shared<AttackActionNode>(0.3f));
//     combatOrRetreat->AddChild(safeAttack);
//
//     // Sub-estratégia: Perseguição cautelosa
//     combatOrRetreat->AddChild(std::make_shared<ChaseActionNode>());
//     defensiveCombat->AddChild(combatOrRetreat);
//     root->AddChild(defensiveCombat);
//
//     // Estratégia 4: Patrulha vigilante
//     root->AddChild(std::make_shared<PatrolActionNode>(120.0f));
//
//     behaviorTree->SetRoot(root);
//     return behaviorTree;
// }
//
// std::shared_ptr<BehaviorTree> EnemyABehaviorTree::CreateMultiStrategyBehaviorTree() {
//     auto behaviorTree = std::make_shared<BehaviorTree>();
//
//     // Behavior Tree com múltiplas estratégias de combate
//     auto root = std::make_shared<SelectorNode>();
//
//     // Estratégia 1: Combate agressivo quando com vida alta
//     auto aggressiveCombat = std::make_shared<SequenceNode>();
//     aggressiveCombat->AddChild(std::make_shared<InverterNode>());
//     aggressiveCombat->GetChildren().back()->AddChild(std::make_shared<IsLowHealthNode>(2.0f));
//     aggressiveCombat->AddChild(std::make_shared<IsPlayerNearNode>(220.0f));
//
//     auto aggressiveStrategy = std::make_shared<SelectorNode>();
//     // Sub-estratégia: Ataque direto
//     auto directAttack = std::make_shared<SequenceNode>();
//     directAttack->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(130.0f));
//     directAttack->AddChild(std::make_shared<AttackActionNode>(0.6f));
//     aggressiveStrategy->AddChild(directAttack);
//
//     // Sub-estratégia: Perseguição agressiva
//     aggressiveStrategy->AddChild(std::make_shared<ChaseActionNode>());
//     aggressiveCombat->AddChild(aggressiveStrategy);
//     root->AddChild(aggressiveCombat);
//
//     // Estratégia 2: Combate defensivo quando com vida baixa
//     auto defensiveCombat = std::make_shared<SequenceNode>();
//     defensiveCombat->AddChild(std::make_shared<IsLowHealthNode>(2.0f));
//     defensiveCombat->AddChild(std::make_shared<IsPlayerNearNode>(180.0f));
//
//     auto defensiveStrategy = std::make_shared<SelectorNode>();
//     // Sub-estratégia: Fuga tática
//     auto tacticalFlee = std::make_shared<SequenceNode>();
//     tacticalFlee->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(100.0f));
//     tacticalFlee->AddChild(std::make_shared<FleeActionNode>(400.0f)); // Fuga tática mais rápida
//     defensiveStrategy->AddChild(tacticalFlee);
//
//     // Sub-estratégia: Ataque à distância
//     auto rangedAttack = std::make_shared<SequenceNode>();
//     rangedAttack->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(120.0f));
//     rangedAttack->AddChild(std::make_shared<CanShootNode>());
//     rangedAttack->AddChild(std::make_shared<AttackActionNode>(0.4f));
//     defensiveStrategy->AddChild(rangedAttack);
//
//     // Sub-estratégia: Perseguição cautelosa
//     defensiveStrategy->AddChild(std::make_shared<ChaseActionNode>());
//     defensiveCombat->AddChild(defensiveStrategy);
//     root->AddChild(defensiveCombat);
//
//     // Estratégia 3: Patrulha inteligente
//     auto intelligentPatrol = std::make_shared<SequenceNode>();
//     intelligentPatrol->AddChild(std::make_shared<InverterNode>());
//     intelligentPatrol->GetChildren().back()->AddChild(std::make_shared<IsPlayerTooFarNode>(300.0f));
//     intelligentPatrol->AddChild(std::make_shared<PatrolActionNode>(150.0f));
//     root->AddChild(intelligentPatrol);
//
//     behaviorTree->SetRoot(root);
//     return behaviorTree;
// }
//
// std::shared_ptr<BehaviorTreeNode> EnemyABehaviorTree::CreateSmartFleeSequence() {
//     auto fleeSequence = std::make_shared<SequenceNode>();
//     fleeSequence->AddChild(std::make_shared<IsLowHealthNode>(1.5f)); // Foge quando tem 1.5 HP ou menos
//     fleeSequence->AddChild(std::make_shared<IsPlayerNearNode>(250.0f)); // Alcance maior de detecção
//     fleeSequence->AddChild(std::make_shared<FleeActionNode>(400.0f)); // Velocidade de fuga MUITO otimizada
//     return fleeSequence;
// }
//
// std::shared_ptr<BehaviorTreeNode> EnemyABehaviorTree::CreateTacticalCombatSequence() {
//     auto combatSequence = std::make_shared<SequenceNode>();
//     combatSequence->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
//
//     auto combatStrategy = std::make_shared<SelectorNode>();
//
//     // Sub-estratégia 1: Ataque à distância quando possível
//     auto rangedCombat = std::make_shared<SequenceNode>();
//     rangedCombat->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(110.0f));
//     rangedCombat->AddChild(std::make_shared<CanShootNode>());
//     rangedCombat->AddChild(std::make_shared<AttackActionNode>(0.4f));
//     combatStrategy->AddChild(rangedCombat);
//
//     // Sub-estratégia 2: Perseguição tática
//     combatStrategy->AddChild(std::make_shared<ChaseActionNode>());
//     combatSequence->AddChild(combatStrategy);
//
//     return combatSequence;
// }
//
// std::shared_ptr<BehaviorTreeNode> EnemyABehaviorTree::CreateAdaptivePatrolSequence() {
//     auto patrolSequence = std::make_shared<SequenceNode>();
//     patrolSequence->AddChild(std::make_shared<InverterNode>());
//     patrolSequence->GetChildren().back()->AddChild(std::make_shared<IsPlayerTooFarNode>(350.0f));
//     patrolSequence->AddChild(std::make_shared<PatrolActionNode>(140.0f)); // Patrulha maior e mais inteligente
//     return patrolSequence;
// }
