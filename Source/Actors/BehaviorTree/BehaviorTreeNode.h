//
// Created by AI Assistant for Behavior Tree implementation
//

#ifndef BEHAVIOR_TREE_NODE_H
#define BEHAVIOR_TREE_NODE_H

#include <vector>
#include <memory>

class Enemy;

// Enum para os possíveis resultados de execução de um nó
enum class NodeResult {
    Success,    // Nó executou com sucesso
    Failure,    // Nó falhou na execução
    Running     // Nó ainda está executando (precisa continuar no próximo frame)
};

// Classe base abstrata para todos os nós da Behavior Tree
class BehaviorTreeNode {
public:
    virtual ~BehaviorTreeNode() = default;
    
    // Método principal que executa o nó
    virtual NodeResult Execute(Enemy* enemy, float deltaTime) = 0;
    
    // Método chamado quando o nó é iniciado (útil para inicializações)
    virtual void OnStart(Enemy* enemy) {}
    
    // Método chamado quando o nó termina (útil para limpeza)
    virtual void OnEnd(Enemy* enemy) {}
    
    // Adiciona um filho ao nó (para nós compostos)
    void AddChild(std::shared_ptr<BehaviorTreeNode> child) {
        mChildren.push_back(child);
    }
    
    // Retorna os filhos do nó
    const std::vector<std::shared_ptr<BehaviorTreeNode>>& GetChildren() const {
        return mChildren;
    }

protected:
    std::vector<std::shared_ptr<BehaviorTreeNode>> mChildren;
};

#endif //BEHAVIOR_TREE_NODE_H
