//
// Created by AI Assistant for Behavior Tree implementation
// Exemplos práticos de Behavior Trees para diferentes tipos de inimigos
//

#include "BehaviorTreeExamples.h"
#include "CompositeNodes.h"
#include "ConditionNodes.h"
#include "ActionNodes.h"
#include <memory>

std::shared_ptr<BehaviorTree> BehaviorTreeExamples::CreateBasicEnemyTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();
    
    // Comportamento básico: Patrulha -> Persegue -> Ataca
    auto root = std::make_shared<SelectorNode>();
    
    // 1. Se jogador próximo e em alcance de ataque, ataca
    auto attackSequence = std::make_shared<SequenceNode>();
    attackSequence->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    attackSequence->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(120.0f));
    attackSequence->AddChild(std::make_shared<AttackActionNode>(0.5f));
    root->AddChild(attackSequence);
    
    // 2. Se jogador próximo mas não em alcance, persegue
    auto chaseSequence = std::make_shared<SequenceNode>();
    chaseSequence->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    chaseSequence->AddChild(std::make_shared<ChaseActionNode>());
    root->AddChild(chaseSequence);
    
    // 3. Caso contrário, patrulha
    root->AddChild(std::make_shared<PatrolActionNode>(100.0f));
    
    behaviorTree->SetRoot(root);
    return behaviorTree;
}

std::shared_ptr<BehaviorTree> BehaviorTreeExamples::CreateAggressiveEnemyTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();
    
    // Comportamento agressivo: Sempre persegue quando detecta o jogador
    auto root = std::make_shared<SelectorNode>();
    
    // 1. Se jogador detectado, persegue agressivamente
    auto aggressiveChase = std::make_shared<SequenceNode>();
    aggressiveChase->AddChild(std::make_shared<IsPlayerNearNode>(300.0f)); // Alcance maior
    auto chaseOrAttack = std::make_shared<SelectorNode>();
    chaseOrAttack->AddChild(std::make_shared<AttackActionNode>(0.3f));
    chaseOrAttack->AddChild(std::make_shared<ChaseActionNode>());
    aggressiveChase->AddChild(chaseOrAttack);
    root->AddChild(aggressiveChase);
    
    // 2. Caso contrário, patrulha rapidamente
    root->AddChild(std::make_shared<PatrolActionNode>(150.0f)); // Patrulha maior
    
    behaviorTree->SetRoot(root);
    return behaviorTree;
}

std::shared_ptr<BehaviorTree> BehaviorTreeExamples::CreateDefensiveEnemyTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();
    
    // Comportamento defensivo: Foge quando ferido, ataca quando saudável
    auto root = std::make_shared<SelectorNode>();
    
    // 1. Se com pouca vida e jogador próximo, foge
    auto fleeSequence = std::make_shared<SequenceNode>();
    fleeSequence->AddChild(std::make_shared<IsLowHealthNode>(1.0f));
    fleeSequence->AddChild(std::make_shared<IsPlayerNearNode>(250.0f));
    fleeSequence->AddChild(std::make_shared<FleeActionNode>(120.0f));
    root->AddChild(fleeSequence);
    
    // 2. Se saudável e jogador próximo, combate normalmente
    auto combatSequence = std::make_shared<SequenceNode>();
    combatSequence->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    auto combatOrChase = std::make_shared<SelectorNode>();
    combatOrChase->AddChild(std::make_shared<AttackActionNode>(0.5f));
    combatOrChase->AddChild(std::make_shared<ChaseActionNode>());
    combatSequence->AddChild(combatOrChase);
    root->AddChild(combatSequence);
    
    // 3. Caso contrário, patrulha cuidadosamente
    root->AddChild(std::make_shared<PatrolActionNode>(80.0f)); // Patrulha menor
    
    behaviorTree->SetRoot(root);
    return behaviorTree;
}

std::shared_ptr<BehaviorTree> BehaviorTreeExamples::CreateSmartEnemyTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();
    
    // Comportamento inteligente: Múltiplas estratégias baseadas na situação
    auto root = std::make_shared<SelectorNode>();
    
    // 1. Estratégia de fuga inteligente
    auto smartFlee = std::make_shared<SequenceNode>();
    smartFlee->AddChild(std::make_shared<IsLowHealthNode>(1.0f));
    smartFlee->AddChild(std::make_shared<IsPlayerNearNode>(200.0f));
    smartFlee->AddChild(std::make_shared<FleeActionNode>(100.0f));
    root->AddChild(smartFlee);
    
    // 2. Estratégia de combate inteligente
    auto smartCombat = std::make_shared<SequenceNode>();
    smartCombat->AddChild(std::make_shared<IsPlayerNearNode>(180.0f));
    auto combatStrategy = std::make_shared<SelectorNode>();
    
    // Sub-estratégia: Ataque à distância
    auto rangedAttack = std::make_shared<SequenceNode>();
    rangedAttack->AddChild(std::make_shared<IsPlayerInAttackRangeNode>(100.0f));
    rangedAttack->AddChild(std::make_shared<CanShootNode>());
    rangedAttack->AddChild(std::make_shared<AttackActionNode>(0.4f));
    combatStrategy->AddChild(rangedAttack);
    
    // Sub-estratégia: Perseguição tática
    combatStrategy->AddChild(std::make_shared<ChaseActionNode>());
    smartCombat->AddChild(combatStrategy);
    root->AddChild(smartCombat);
    
    // 3. Estratégia de patrulha vigilante
    auto vigilantPatrol = std::make_shared<SequenceNode>();
    vigilantPatrol->AddChild(std::make_shared<InverterNode>());
    vigilantPatrol->GetChildren().back()->AddChild(std::make_shared<IsPlayerTooFarNode>(350.0f));
    vigilantPatrol->AddChild(std::make_shared<PatrolActionNode>(120.0f));
    root->AddChild(vigilantPatrol);
    
    behaviorTree->SetRoot(root);
    return behaviorTree;
}

std::shared_ptr<BehaviorTree> BehaviorTreeExamples::CreateSuicidalEnemyTree() {
    auto behaviorTree = std::make_shared<BehaviorTree>();
    
    // Comportamento suicida: Sempre ataca quando possível, nunca foge
    auto root = std::make_shared<SelectorNode>();
    
    // 1. Se jogador detectado, sempre tenta atacar
    auto suicidalAttack = std::make_shared<SequenceNode>();
    suicidalAttack->AddChild(std::make_shared<IsPlayerNearNode>(250.0f));
    auto attackOrRush = std::make_shared<SelectorNode>();
    attackOrRush->AddChild(std::make_shared<AttackActionNode>(0.8f)); // Ataque mais longo
    attackOrRush->AddChild(std::make_shared<ChaseActionNode>());
    suicidalAttack->AddChild(attackOrRush);
    root->AddChild(suicidalAttack);
    
    // 2. Caso contrário, patrulha agressivamente
    root->AddChild(std::make_shared<PatrolActionNode>(200.0f)); // Patrulha muito grande
    
    behaviorTree->SetRoot(root);
    return behaviorTree;
}
