//
// Created by AI Assistant for Behavior Tree implementation
//

#include "EnemyBehaviorTree.h"
#include "CompositeNodes.h"
#include "ConditionNodes.h"
#include "ActionNodes.h"
#include <memory>

std::shared_ptr<BehaviorTree> EnemyBehaviorTree::CreateBasicBehaviorTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();
    
    // Cria a raiz como um Selector (tenta diferentes estratégias)
    auto root = std::make_shared<SelectorNode>();
    
    // Estratégia 1: Combate (se jogador próximo e em alcance de ataque)
    auto combatSequence = CreateCombatSequence();
    root->AddChild(combatSequence);
    
    // Estratégia 2: Perseguição (se jogador próximo mas não em alcance de ataque)
    auto chaseSequence = std::make_shared<SequenceNode>();
    chaseSequence->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    chaseSequence->AddChild(std::make_shared<ChaseActionNode>());
    root->AddChild(chaseSequence);
    
    // Estratégia 3: Patrulha (comportamento padrão)
    auto patrolSequence = CreatePatrolSequence();
    root->AddChild(patrolSequence);
    
    behaviorTree->SetRoot(root);
    return behaviorTree;
}

std::shared_ptr<BehaviorTree> EnemyBehaviorTree::CreateAdvancedBehaviorTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();
    
    // Cria a raiz como um Selector com comportamentos mais complexos
    auto root = std::make_shared<SelectorNode>();
    
    // Estratégia 1: Fuga (se com pouca vida)
    auto fleeSequence = CreateFleeSequence();
    root->AddChild(fleeSequence);
    
    // Estratégia 2: Combate agressivo (se jogador próximo e pode atirar)
    auto aggressiveCombat = std::make_shared<SequenceNode>();
    aggressiveCombat->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    aggressiveCombat->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(120.0f));
    aggressiveCombat->AddChild(std::make_shared<CanShootNode>());
    aggressiveCombat->AddChild(std::make_shared<AttackActionNode>(0.5f));
    root->AddChild(aggressiveCombat);
    
    // Estratégia 3: Perseguição inteligente (se jogador próximo mas não pode atacar)
    auto smartChase = std::make_shared<SequenceNode>();
    smartChase->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    auto chaseOrAttack = std::make_shared<SelectorNode>();
    chaseOrAttack->AddChild(std::make_shared<AttackActionNode>(0.3f));
    chaseOrAttack->AddChild(std::make_shared<ChaseActionNode>());
    smartChase->AddChild(chaseOrAttack);
    root->AddChild(smartChase);
    
    // Estratégia 4: Patrulha vigilante (comportamento padrão)
    auto vigilantPatrol = std::make_shared<SequenceNode>();
    vigilantPatrol->AddChild(std::make_shared<InverterNode>());
    vigilantPatrol->GetChildren().back()->AddChild(std::make_shared<IsPlayerTooFarNode>(400.0f));
    vigilantPatrol->AddChild(std::make_shared<PatrolActionNode>(100.0f));
    root->AddChild(vigilantPatrol);
    
    behaviorTree->SetRoot(root);
    return behaviorTree;
}

std::shared_ptr<BehaviorTree> EnemyBehaviorTree::CreateLowHealthBehaviorTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();
    
    auto root = std::make_shared<SelectorNode>();
    
    // Estratégia 1: Fuga desesperada (prioridade máxima)
    auto desperateFlee = std::make_shared<SequenceNode>();
    desperateFlee->AddChild(std::make_shared<IsPlayerNearNode>(300.0f)); // Maior alcance de detecção
    desperateFlee->AddChild(std::make_shared<FleeActionNode>(120.0f)); // Velocidade maior de fuga
    root->AddChild(desperateFlee);
    
    // Estratégia 2: Ataque suicida (se não conseguir fugir)
    auto suicideAttack = std::make_shared<SequenceNode>();
    suicideAttack->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(150.0f)); // Alcance maior
    suicideAttack->AddChild(std::make_shared<AttackActionNode>(0.8f)); // Ataque mais longo
    root->AddChild(suicideAttack);
    
    // Estratégia 3: Ficar parado e se recuperar
    root->AddChild(std::make_shared<IdleActionNode>());
    
    behaviorTree->SetRoot(root);
    return behaviorTree;
}

std::shared_ptr<BehaviorTree> EnemyBehaviorTree::CreateBehaviorTreeForType(int enemyType) {
    switch (enemyType) {
        case 0: // Inimigo tipo A - Behavior Tree inteligente
            return CreateAdvancedBehaviorTree();
        case 1: // Inimigo tipo B - Usa FSM (não deveria chamar esta função)
            return CreateBasicBehaviorTree();
        default:
            return CreateBasicBehaviorTree();
    }
}

std::shared_ptr<BehaviorTreeNode> EnemyBehaviorTree::CreateCombatSequence() {
    auto combatSequence = std::make_shared<SequenceNode>();
    combatSequence->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    combatSequence->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(120.0f));
    combatSequence->AddChild(std::make_shared<AttackActionNode>(0.5f));
    return combatSequence;
}

std::shared_ptr<BehaviorTreeNode> EnemyBehaviorTree::CreatePatrolSequence() {
    auto patrolSequence = std::make_shared<SequenceNode>();
    patrolSequence->AddChild(std::make_shared<InverterNode>());
    patrolSequence->GetChildren().back()->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    patrolSequence->AddChild(std::make_shared<PatrolActionNode>(100.0f));
    return patrolSequence;
}

std::shared_ptr<BehaviorTreeNode> EnemyBehaviorTree::CreateFleeSequence() {
    auto fleeSequence = std::make_shared<SequenceNode>();
    fleeSequence->AddChild(std::make_shared<IsLowHealthNode>(1.0f));
    fleeSequence->AddChild(std::make_shared<IsPlayerNearNode>(250.0f));
    fleeSequence->AddChild(std::make_shared<FleeActionNode>(100.0f));
    return fleeSequence;
}
