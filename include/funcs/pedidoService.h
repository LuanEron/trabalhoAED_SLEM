#ifndef FUNC_pedidoService_H
#define FUNC_pedidoService_H

#include <iostream>
#include <vector>
#include "classes/Endereco.h"
#include "classes/Local.h"
#include "classes/Veiculo.h"
#include "funcs/localService.h"
#include "database/repositorio.h" 

/**
 * @class PedidoService
 * @brief Classe responsável por gerenciar as operações CRUD para pedidos.
 * * Esta classe fornece métodos para criar, ler, atualizar e deletar pedidos,
 * utilizando o repositório de dados.
 */
class PedidoService {
    private:
        Repositorio* repositorio; // Ponteiro para o repositório onde os pedidos são armazenados
    public:
        PedidoService() = default; // Construtor padrão
        PedidoService(Repositorio* repo) : repositorio(repo) {} // Construtor que recebe um repositório
        
        // ================== CRUD PARA PEDIDOS ==================
        /**
         * @brief Cria um novo pedido com os dados fornecidos.
         * * @param origem Local de origem do pedido.
         * @param destino Local de destino do pedido.
         * @param peso Peso do pedido em quilogramas.
         * @param status Status do pedido (false = pendente, true = concluído).
         * @return int ID do pedido criado, ou -1 se o peso for inválido.
         */
        int criarPedido(const Local& origem, const Local& destino, float peso, bool status) {
            Pedido novo(origem, destino, peso, status);
            if(!novo.setOrigem(origem)) return -1;
            if(!novo.setDestino(destino)) return -1;
            if(!novo.setPeso(peso)) return -1; // Retorna -1 se o peso for inválido
            novo.setStatus(false); // Define o status como pendente por padrão
            return repositorio->addPedido(novo);
        }

        /**
         * @brief Lê um pedido pelo ID.
         * * @param id ID do pedido a ser lido.
         * @return Pedido* Ponteiro para o objeto Pedido, ou nullptr se não encontrado.
         */
        Pedido* lerPedido(int id) {
            return repositorio->getPedido(id);
        }

        /**
         * @brief Atualiza um pedido existente com os novos dados fornecidos.
         * * @param id ID do pedido a ser atualizado.
         * @param origem Novo local de origem do pedido.
         * @param destino Novo local de destino do pedido.
         * @param peso Novo peso do pedido em quilogramas.
         * @return true se a atualização for bem-sucedida, false caso contrário.
         */
        bool atualizarPedido(const int id, const Local& origem, const Local& destino, float peso) {
            Pedido* pedido = repositorio->getPedido(id);
            if (!pedido) return false; // Pedido não encontrado

            pedido->setOrigem(origem);
            pedido->setDestino(destino);
            pedido->setPeso(peso);
            repositorio->updatePedido(*pedido);
            return true;
        }

        /**
         * @brief Deleta um pedido pelo ID.
         * * @param id ID do pedido a ser deletado.
         * @return true se a deleção for bem-sucedida, false caso contrário.
         */
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

/**
 * @brief Função para adicionar um novo pedido ao repositório.
 * @param repo Ponteiro para o repositório onde o pedido será adicionado.
 * Esta função solicita ao usuário as informações do pedido, como locais de origem e destino e peso,
 * e utiliza a classe PedidoService para criar o pedido e adicioná-lo ao repositório.
 */
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

/**
 * @brief Função para listar todos os pedidos no repositório.
 * @param repo Ponteiro para o repositório de onde os pedidos serão listados.
 * Esta função obtém todos os pedidos do repositório e os exibe no console.
 */
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

/**
 * @brief Função para listar todos os pedidos pendentes no repositório.
 * @param repo Ponteiro para o repositório de onde os pedidos pendentes serão listados.
 */
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

/**
 * @brief Função para listar todos os pedidos entregues no repositório.
 * @param repo Ponteiro para o repositório de onde os pedidos entregues serão listados.
 */
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

/**
 * @brief Função para editar um pedido existente no repositório.
 * @param repo Ponteiro para o repositório onde o pedido será editado.
 * Esta função solicita ao usuário o ID do pedido a ser editado e as novas informações do pedido,
 * e utiliza a classe PedidoService para atualizar o pedido no repositório.
 */
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

/**
 * @brief Função para remover um pedido do repositório.
 * @param repo Ponteiro para o repositório de onde o pedido será removido. 
 * Esta função solicita ao usuário o ID do pedido a ser removido
 * e utiliza a classe PedidoService para deletar o pedido do repositório.
 */
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
