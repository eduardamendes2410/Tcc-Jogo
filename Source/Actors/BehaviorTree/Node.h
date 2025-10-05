//
// Created by eduar on 28/09/2025.
//

#ifndef NODE_H
#define NODE_H

// Enum para os possíveis status de um nó
enum class NodeStatus {
    Success,
    Failure,
    Running
};

// Classe base para todos os nós da árvore
class Node {
public:
    virtual ~Node() = default;
    virtual NodeStatus Tick() = 0; // O "coração" da árvore, executado a cada frame

    // Hooks opcionais que nós podem sobrescrever para gerenciar estados internos
    virtual void Initialize() {}
    virtual void Terminate(NodeStatus) {}
};



#endif //NODE_H
