#ifndef FUNC_pedidoService_H
#define FUNC_pedidoService_H

#include <iostream>
#include "classes/Endereco.h"
#include "classes/Local.h"
#include "classes/Veiculo.h"
#include "funcs/localService.h"
#include "database/repositorio.h" 

class PedidoService {
    private:
        Repositorio* repositorio;
    public:
        PedidoService() = default;
        PedidoService(Repositorio* repo) : repositorio(repo) {}
        
        // ================== CRUD PARA PEDIDOS ==================
        int criarPedido(const Local& origem, const Local& destino, float peso, bool status) {
            Pedido novo(origem, destino, peso, status);
            if(!novo.setOrigem(origem)) return -1;
            if(!novo.setDestino(destino)) return -1;
            if(!novo.setPeso(peso)) return -1; // Retorna -1 se o peso for inválido
            novo.setStatus(false); // Define o status como pendente por padrão
            return repositorio->addPedido(novo);
        }

        Pedido* lerPedido(int id) {
            return repositorio->getPedido(id);
        }

        bool atualizarPedido(const int id, const Local& origem, const Local& destino, float peso) {
            Pedido* pedido = repositorio->getPedido(id);
            if (!pedido) return false; // Pedido não encontrado

            pedido->setOrigem(origem);
            pedido->setDestino(destino);
            pedido->setPeso(peso);
            repositorio->updatePedido(*pedido);
            return true;
        }

        bool deletarPedido(int id) {
            try {
                repositorio->removePedido(id);
                return true;
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro ao deletar pedido: " << e.what() << std::endl;
                return false;
            }
        }
};

void adicionarPedido(Repositorio* repo) {
     PedidoService pedidoService(repo);
    Local origem, destino;
    float peso;

    listarLocais(repo);
    std::cout << "Digite o ID do local de origem: ";
    int origemId;
    std::cin >> origemId;
    Local* localOrigem = repo->getLocal(origemId);
    if (localOrigem) {
        origem = *localOrigem;
    } else {
        std::cerr << "Local de origem não encontrado." << std::endl;
        return;
    }

    listarLocais(repo);
    std::cout << "Digite o ID do local de destino: ";
    int destinoId;
    std::cin >> destinoId;
    Local* localDestino = repo->getLocal(destinoId);
    if (localDestino == localOrigem) {
        std::cerr << "O local de destino não pode ser o mesmo que o de origem." << std::endl;
        return;
    }
    if (localDestino) {
        destino = *localDestino;
    } else {
        std::cerr << "Local de destino não encontrado." << std::endl;
        return;
    }

    std::cout << "Digite o peso do pedido (Kg): ";
    std::cin >> peso;

    int id = pedidoService.criarPedido(origem, destino, peso, false);
    if (id != -1) {
        std::cout << "Pedido adicionado com sucesso! ID: " << id << std::endl;
    } else {
        std::cerr << "Erro ao adicionar pedido." << std::endl;
    }
}

void listarPedidos(Repositorio* repo) {
    std::cout << "Lista de pedidos:" << std::endl;
    Pedido* pedidos = repo->getAllPedido();
    int numPedidos = repo->getNumPedidos();

    if (numPedidos == 0) {
        std::cout << "Nenhum pedido cadastrado." << std::endl;
        return;
    }

    for (int i = 0; i < numPedidos; ++i) {
        const auto& pedido = pedidos[i];
        std::cout << "ID: " << pedido.getId() 
                  << ", Origem: " << pedido.getOrigem().getEndereco().getRua() 
                  << ", Destino: " << pedido.getDestino().getEndereco().getRua() 
                  << ", Peso: " << pedido.getPeso() 
                  << " Kg, Status: " << (pedido.getStatus() ? "Entregue" : "Pendente") 
                  << std::endl;
    }
}

void listarPedidosPendentes(Repositorio* repo) {
    std::cout << "Lista de pedidos pendentes:" << std::endl;
    Pedido* pedidos = repo->getAllPedido();
    int numPedidos = repo->getNumPedidos();
    bool encontrou = false;

    for (int i = 0; i < numPedidos; ++i) {
        const auto& pedido = pedidos[i];
        if (!pedido.getStatus()) { // Verifica se o pedido está pendente
            std::cout << "ID: " << pedido.getId() 
                      << ", Origem: " << pedido.getOrigem().getEndereco().getRua() 
                      << ", Destino: " << pedido.getDestino().getEndereco().getRua() 
                      << ", Peso: " << pedido.getPeso() 
                      << " Kg" << std::endl;
            encontrou = true;
        }
    }
    if (!encontrou) {
        std::cout << "Nenhum pedido pendente." << std::endl;
    }
}

void listarPedidosEntregues(Repositorio* repo) {
    std::cout << "Lista de pedidos entregues:" << std::endl;
    Pedido* pedidos = repo->getAllPedido();
    int numPedidos = repo->getNumPedidos();
    bool encontrou = false;

    for (int i = 0; i < numPedidos; ++i) {
        const auto& pedido = pedidos[i];
        if (pedido.getStatus()) { // Verifica se o pedido está entregue
            std::cout << "ID: " << pedido.getId() 
                      << ", Origem: " << pedido.getOrigem().getEndereco().getRua() 
                      << ", Destino: " << pedido.getDestino().getEndereco().getRua() 
                      << ", Peso: " << pedido.getPeso() 
                      << " Kg" << std::endl;
            encontrou = true;
        }
    }
    if (!encontrou) {
        std::cout << "Nenhum pedido entregue." << std::endl;
    }
}   

void editarPedido(Repositorio* repo) {
    PedidoService pedidoService(repo);
    int id;
    Local origem, destino;
    float peso;

    std::cout << "Digite o ID do pedido a ser editado: ";
    std::cin >> id;

    Pedido* pedido = pedidoService.lerPedido(id);
    if (!pedido) {
        std::cerr << "Pedido não encontrado." << std::endl;
        return;
    }

    listarLocais(repo);
    std::cout << "Digite o ID do novo local de origem: ";
    int origemId;
    std::cin >> origemId;
    Local* localOrigem = repo->getLocal(origemId);
    if (localOrigem) {
        origem = *localOrigem;
    } else {
        std::cerr << "Local de origem não encontrado." << std::endl;
        return;
    }
    
    listarLocais(repo);
    std::cout << "Digite o ID do novo local de destino: ";
    int destinoId;
    std::cin >> destinoId;
    Local* localDestino = repo->getLocal(destinoId);
    if (localDestino) {
        destino = *localDestino;
    } else {
        std::cerr << "Local de destino não encontrado." << std::endl;
        return;
    }

    std::cout << "Digite o novo peso do pedido (Kg): ";
    std::cin >> peso;

    if (pedidoService.atualizarPedido(id, origem, destino, peso)) {
        std::cout << "Pedido atualizado com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro ao atualizar pedido." << std::endl;
    }
}

void removerPedido(Repositorio* repo) {
    PedidoService pedidoService(repo);
    int id;

    std::cout << "Digite o ID do pedido a ser removido: ";
    std::cin >> id;

    if (pedidoService.deletarPedido(id)) {
        std::cout << "Pedido removido com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro ao remover pedido." << std::endl;
    }
}

#endif // FUNC_pedidoService_H
