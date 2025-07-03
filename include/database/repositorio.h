Com certeza! Peço desculpas se a versão anterior se baseou em arquivos incorretos. Fico feliz em ajudar a ajustar o código que você enviou agora.

Entendi perfeitamente as regras. Vou refatorar os arquivos para substituir o std::vector por arrays e contadores, mantendo toda a estrutura, lógica e, principalmente, o padrão de comentários que você estabeleceu.

A seguir, apresento as versões completas e atualizadas dos arquivos que precisam de modificação. Você pode simplesmente substituir o conteúdo dos seus arquivos por estes. Os arquivos main.cpp e os headers das classes (Local.h, Veiculo.h, etc.) não precisam de nenhuma alteração.

Arquivo: repositorio.h (Atualizado)
Esta é a mudança central. Os vetores foram trocados por arrays de tamanho fixo e adicionei os contadores. Todas as funções que manipulavam os vetores (add, get, remove, update, salvarBinario, carregarBinario) foram reescritas para usar a nova estrutura.

C++

#ifndef REPOSITORIO_H
#define REPOSITORIO_H

#pragma once
#include "classes/Local.h"
#include "classes/Veiculo.h"
#include "classes/Pedido.h"
#include <string>
#include <fstream>
#include <stdexcept>

/**
 * @class Repositorio
 * @brief Classe responsável por gerenciar os dados do sistema, incluindo locais, veículos e pedidos.
 * * Esta classe fornece métodos para adicionar, atualizar, remover e consultar locais, veículos e pedidos.
 * Os dados são armazenados em arrays e podem ser salvos e carregados de um arquivo binário.
 */
class Repositorio {
private:
    /**
     * @brief Define a capacidade máxima de armazenamento para cada tipo de entidade.
     */
    static const int MAX_ENTIDADES = 100;

    // Arrays para armazenar os dados
    Local locais[MAX_ENTIDADES];
    Veiculo veiculos[MAX_ENTIDADES];
    Pedido pedidos[MAX_ENTIDADES];

    /**
     * @brief Contador para o número atual de locais armazenados.
     */
    int numLocais;
    /**
     * @brief Contador para o número atual de veículos armazenados.
     */
    int numVeiculos;
    /**
     * @brief Contador para o número atual de pedidos armazenados.
     */
    int numPedidos;

    // Contadores para IDs únicos
    int nextLocalId;
    int nextVeiculoId;
    int nextPedidoId;

public:
    /**
     * @brief Construtor padrão da classe Repositorio.
     * @details Inicializa os contadores de entidades e IDs como 0.
     */
    Repositorio() : numLocais(0), numVeiculos(0), numPedidos(0), nextLocalId(0), nextVeiculoId(0), nextPedidoId(0) {}

    // Locais
    /**
     * @brief Gera o próximo ID único para um local.
     * @return int Próximo ID único para um local.
     */
    int gerarProximoIdLocal() {
        return nextLocalId++;
    }

    /**
     * @brief Adiciona um novo local ao repositório.
     * @param l Objeto Local a ser adicionado.
     * @return int ID do local adicionado, ou -1 se o repositório estiver cheio.
     */
    int addLocal(Local l) { 
        if (numLocais >= MAX_ENTIDADES) {
            return -1; // Repositório cheio
        }
        l.setId(gerarProximoIdLocal()); 
        locais[numLocais++] = l; 
        return l.getId();
    }
    
    /**
     * @brief Obtém um local pelo ID.
     * @param id ID do local a ser obtido.
     * @return Local* Ponteiro para o objeto Local, ou nullptr se não encontrado.
     */
    Local* getLocal(int id) {
        for (int i = 0; i < numLocais; ++i) {
            if (locais[i].getId() == id) {
                return &locais[i];
            }
        }
        return nullptr;
    }

    /**
     * @brief Atualiza um local existente no repositório.
     * @param elemento Objeto Local com os dados atualizados.
     * @throws std::runtime_error Se o local não for encontrado.
     */
    void updateLocal(const Local& elemento) {
        for (int i = 0; i < numLocais; ++i) {
            if (locais[i].getId() == elemento.getId()) {
                locais[i] = elemento;
                return;
            }
        }
        throw std::runtime_error("Local não encontrado");
    }

    /**
     * @brief Remove um local do repositório pelo ID.
     * @param id ID do local a ser removido.
     * @throws std::runtime_error Se o local não for encontrado.
     */
    void removeLocal(int id)  {
        for (int i = 0; i < numLocais; ++i) {
            if (locais[i].getId() == id) {
                // Desloca os elementos para preencher o espaço
                for (int j = i; j < numLocais - 1; ++j) {
                    locais[j] = locais[j + 1];
                }
                numLocais--;
                return;
            }
        }
        throw std::runtime_error("Local não encontrado");
    }

    /**
     * @brief Obtém um ponteiro para o array de todos os locais.
     * @return Local* Ponteiro para o início do array de locais.
     */
    Local* getAllLocal() {
        return locais;
    }

    /**
     * @brief Obtém o número total de locais cadastrados.
     * @return int O número de locais.
     */
    int getNumLocais() const {
        return numLocais;
    }

    // Veículos
    /**
     * @brief Gera o próximo ID único para um veículo.
     * @return int Próximo ID único para um veículo.
     */
    int gerarProximoIdVeiculo() {
        return nextVeiculoId++;
    }

    /**
     * @brief Adiciona um novo veículo ao repositório.
     * @param v Objeto Veiculo a ser adicionado.
     * @return int ID do veículo adicionado, ou -1 se o repositório estiver cheio.
     */
    int addVeiculo(Veiculo v) { 
        if (numVeiculos >= MAX_ENTIDADES) {
            return -1; // Repositório cheio
        }
        v.setId(gerarProximoIdVeiculo()); 
        veiculos[numVeiculos++] = v;
        return v.getId();
    }
    
     /**
      * @brief Obtém um veículo pela placa.
      * @param placa Placa do veículo a ser obtido.
      * @return Veiculo* Ponteiro para o objeto Veiculo, ou nullptr se não encontrado.
      */
    Veiculo* getVeiculo(const char* placa) { 
        for (int i = 0; i < numVeiculos; ++i) {
            if (strcmp(veiculos[i].getPlaca(), placa) == 0) {
                return &veiculos[i];
            }
        }
        return nullptr;
     }

    /**
     * @brief Atualiza um veículo existente no repositório.
     * @param elemento Objeto Veiculo com os dados atualizados.
     * @throws std::runtime_error Se o veículo não for encontrado.
     */
    void updateVeiculo(const Veiculo& elemento) {
        for (int i = 0; i < numVeiculos; ++i) {
            if (strcmp(veiculos[i].getPlaca(), elemento.getPlaca()) == 0) {
                veiculos[i] = elemento;
                return;
            }
        }
        throw std::runtime_error("Veículo não encontrado");
    }

    /**
     * @brief Remove um veículo do repositório pela placa.
     * @param placa Placa do veículo a ser removido.
     * @throws std::runtime_error Se o veículo não for encontrado.
     */
    void removeVeiculo(const char* placa) {
        for (int i = 0; i < numVeiculos; ++i) {
            if (strcmp(veiculos[i].getPlaca(), placa) == 0) {
                for (int j = i; j < numVeiculos - 1; ++j) {
                    veiculos[j] = veiculos[j + 1];
                }
                numVeiculos--;
                return;
            }
        }
        throw std::runtime_error("Veículo não encontrado");
    }

    /**
     * @brief Obtém um ponteiro para o array de todos os veículos.
     * @return Veiculo* Ponteiro para o início do array de veículos.
     */
    Veiculo* getAllVeiculo() {
        return veiculos;
    }
    
    /**
     * @brief Obtém o número total de veículos cadastrados.
     * @return int O número de veículos.
     */
    int getNumVeiculos() const {
        return numVeiculos;
    }

    // Pedidos
    /**
     * @brief Gera o próximo ID único para um pedido.
     * @return int Próximo ID único para um pedido.
     */
    int gerarProximoIdPedido() {
        return nextPedidoId++;
    }

    /**
     * @brief Adiciona um novo pedido ao repositório.
     * @param p Objeto Pedido a ser adicionado.
     * @return int ID do pedido adicionado, ou -1 se o repositório estiver cheio.
     */
    int addPedido(Pedido p) { 
        if (numPedidos >= MAX_ENTIDADES) {
            return -1; // Repositório cheio
        }
        p.setId(gerarProximoIdPedido()); 
        pedidos[numPedidos++] = p;
        return p.getId();
    }
    
    /**
     * @brief Obtém um pedido pelo ID.
     * @param id ID do pedido a ser obtido.
     * @return Pedido* Ponteiro para o objeto Pedido, ou nullptr se não encontrado.
     */
    Pedido* getPedido(int id) { 
        for (int i = 0; i < numPedidos; ++i) {
            if (pedidos[i].getId() == id) {
                return &pedidos[i];
            }
        }
        return nullptr;
    }

    /**
     * @brief Atualiza um pedido existente no repositório.
     * @param elemento Objeto Pedido com os dados atualizados.
     * @throws std::runtime_error Se o pedido não for encontrado.
     */
    void updatePedido(const Pedido& elemento) {
        for (int i = 0; i < numPedidos; ++i) {
            if (pedidos[i].getId() == elemento.getId()) {
                pedidos[i] = elemento;
                return;
            }
        }
        throw std::runtime_error("Pedido não encontrado");
    }

    /**
     * @brief Remove um pedido do repositório pelo ID.
     * @param id ID do pedido a ser removido.
     * @throws std::runtime_error Se o pedido não for encontrado.
     */
    void removePedido(int id) {
        for (int i = 0; i < numPedidos; ++i) {
            if (pedidos[i].getId() == id) {
                for (int j = i; j < numPedidos - 1; ++j) {
                    pedidos[j] = pedidos[j + 1];
                }
                numPedidos--;
                return;
            }
        }
        throw std::runtime_error("Pedido não encontrado");
    }

    /**
     * @brief Obtém um ponteiro para o array de todos os pedidos.
     * @return Pedido* Ponteiro para o início do array de pedidos.
     */
    Pedido* getAllPedido() {
        return pedidos;
    }

    /**
     * @brief Obtém o número total de pedidos cadastrados.
     * @return int O número de pedidos.
     */
    int getNumPedidos() const {
        return numPedidos;
    }
    
    /**
     * @brief Salva os dados do repositório em um arquivo binário.
     * * @param DB_PATH Caminho do arquivo onde os dados serão salvos.
     * @details Esta função salva todos os dados do repositório, incluindo locais, veículos e pedidos,
     * em um arquivo binário especificado pelo caminho DB_PATH. O formato do arquivo
     * é estruturado para facilitar a leitura e escrita dos dados, garantindo que os IDs
     * sejam mantidos corretamente entre as execuções do programa.
     * @throws std::runtime_error Se houver um erro ao abrir o arquivo para escrita.
     */
    void salvarBinario(const char* DB_PATH) {
        std::ofstream outFile(DB_PATH, std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Erro ao abrir o arquivo para escrita");
        }

        outFile.write(reinterpret_cast<const char*>(&numLocais), sizeof(numLocais));
        for (int i = 0; i < numLocais; ++i) {
            outFile.write(reinterpret_cast<const char*>(&locais[i]), sizeof(Local));
        }

        outFile.write(reinterpret_cast<const char*>(&numVeiculos), sizeof(numVeiculos));
        for (int i = 0; i < numVeiculos; ++i) {
            outFile.write(reinterpret_cast<const char*>(&veiculos[i]), sizeof(Veiculo));
        }

        outFile.write(reinterpret_cast<const char*>(&numPedidos), sizeof(numPedidos));
        for (int i = 0; i < numPedidos; ++i) {
            outFile.write(reinterpret_cast<const char*>(&pedidos[i]), sizeof(Pedido));
        }

        outFile.close();
    }

    /**
     * @brief Carrega os dados do repositório a partir de um arquivo binário.
     * * @param DB_PATH Caminho do arquivo de onde os dados serão carregados.
     * @details Esta função lê os dados do repositório a partir de um arquivo binário especificado pelo caminho DB_PATH.
     * Ela restaura os locais, veículos e pedidos, garantindo que os IDs sejam mantidos corretamente entre as execuções do programa.
     * @throws std::runtime_error Se houver um erro ao abrir o arquivo para leitura ou se os dados estiverem corrompidos.
     */
    void carregarBinario(const char* DB_PATH) {
        std::ifstream inFile(DB_PATH, std::ios::binary);
        if (!inFile) {
            // Se o arquivo não existe, não é um erro, apenas inicia vazio.
            return;
        }

        // Carrega locais
        inFile.read(reinterpret_cast<char*>(&numLocais), sizeof(numLocais));
        if (numLocais < 0 || numLocais > MAX_ENTIDADES) { numLocais = 0; return; }
        for (int i = 0; i < numLocais; ++i) {
            inFile.read(reinterpret_cast<char*>(&locais[i]), sizeof(Local));
        }
        int maxLocalId = 0;
        for(int i = 0; i < numLocais; ++i) if(locais[i].getId() > maxLocalId) maxLocalId = locais[i].getId();
        nextLocalId = (numLocais > 0) ? maxLocalId + 1 : 0;

        // Carrega veículos
        inFile.read(reinterpret_cast<char*>(&numVeiculos), sizeof(numVeiculos));
        if (numVeiculos < 0 || numVeiculos > MAX_ENTIDADES) { numVeiculos = 0; return; }
        for (int i = 0; i < numVeiculos; ++i) {
            inFile.read(reinterpret_cast<char*>(&veiculos[i]), sizeof(Veiculo));
        }
        int maxVeiculoId = 0;
        for(int i = 0; i < numVeiculos; ++i) if(veiculos[i].getId() > maxVeiculoId) maxVeiculoId = veiculos[i].getId();
        nextVeiculoId = (numVeiculos > 0) ? maxVeiculoId + 1 : 0;

        // Carrega pedidos
        inFile.read(reinterpret_cast<char*>(&numPedidos), sizeof(numPedidos));
        if (numPedidos < 0 || numPedidos > MAX_ENTIDADES) { numPedidos = 0; return; }
        for (int i = 0; i < numPedidos; ++i) {
            inFile.read(reinterpret_cast<char*>(&pedidos[i]), sizeof(Pedido));
        }
        int maxPedidoId = 0;
        for(int i = 0; i < numPedidos; ++i) if(pedidos[i].getId() > maxPedidoId) maxPedidoId = pedidos[i].getId();
        nextPedidoId = (numPedidos > 0) ? maxPedidoId + 1 : 0;

        inFile.close();
    }
};

#endif // REPOSITORIO_H
