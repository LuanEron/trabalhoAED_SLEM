#ifndef REPOSITORIO_H
#define REPOSITORIO_H

#pragma once
#include "classes/Local.h"
#include "classes/Veiculo.h"
#include "classes/Pedido.h"
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept> // Para std::runtime_error

class Repositorio {
    std::vector<Local> locais;
    std::vector<Veiculo> veiculos;
    std::vector<Pedido> pedidos;
private:
    int nextLocalId = 0; // Para IDs únicos de locais
    int nextVeiculoId = 0; // Para IDs únicos de veículos
    int nextPedidoId = 0; // Para IDs únicos de pedidos

public:
    // Locais
    int gerarProximoIdLocal() {
        return nextLocalId++;
    }

    int addLocal(const Local& l) {
        Local novoLocal = l;
        novoLocal.setId(gerarProximoIdLocal());
        locais.push_back(novoLocal);
        return novoLocal.getId();
    }

    Local* getLocal(int id) {
        for (auto& local : locais) {
            if (local.getId() == id) {
                return &local;
            }
        }
        return nullptr;
    }

    void updateLocal(const Local& elemento) {
        for (auto& local : locais) {
            if (local.getId() == elemento.getId()) {
                local = elemento;
                return;
            }
        }
        throw std::runtime_error("Local não encontrado para atualização");
    }

    void removeLocal(int id)  {
        for (auto it = locais.begin(); it != locais.end(); ++it) {
            if (it->getId() == id) {
                locais.erase(it);
                return;
            }
        }
        throw std::runtime_error("Local não encontrado para remoção");
    }

    std::vector<Local>& getAllLocal() {
        return locais;
    }

    // Veículos
    int gerarProximoIdVeiculo() {
        return nextVeiculoId++;
    }

    int addVeiculo(const Veiculo& v) {
        Veiculo novoVeiculo = v;
        novoVeiculo.setId(gerarProximoIdVeiculo());
        veiculos.push_back(novoVeiculo);
        return novoVeiculo.getId();
    }

    Veiculo* getVeiculo(const char* placa) {
        for (auto& veiculo : veiculos) {
            if (strcmp(veiculo.getPlaca(), placa) == 0) {
                return &veiculo;
            }
        }
        return nullptr;
     }

    void updateVeiculo(const Veiculo& elemento) {
        for (auto& veiculo : veiculos) {
            if (strcmp(veiculo.getPlaca(), elemento.getPlaca()) == 0) {
                veiculo = elemento;
                return;
            }
        }
        throw std::runtime_error("Veículo não encontrado para atualização");
    }

    void removeVeiculo(const char* placa) {
        for (auto it = veiculos.begin(); it != veiculos.end(); ++it) {
            if (strcmp(it->getPlaca(), placa) == 0) {
                veiculos.erase(it);
                return;
            }
        }
        throw std::runtime_error("Veículo não encontrado para remoção");
    }

    std::vector<Veiculo>& getAllVeiculo() {
        return veiculos;
    }

    // Pedidos
    int gerarProximoIdPedido() {
        return nextPedidoId++;
    }

    int addPedido(const Pedido& p) {
        Pedido novoPedido = p;
        novoPedido.setId(gerarProximoIdPedido());
        pedidos.push_back(novoPedido);
        return novoPedido.getId();
    }

    Pedido* getPedido(int id) {
        for (auto& pedido : pedidos) {
            if (pedido.getId() == id) {
                return &pedido;
            }
        }
        return nullptr;
    }

    void updatePedido(const Pedido& elemento) {
        for (auto& pedido : pedidos) {
            if (pedido.getId() == elemento.getId()) {
                pedido = elemento;
                return;
            }
        }
        throw std::runtime_error("Pedido não encontrado para atualização");
    }

    void removePedido(int id) {
        for (auto it = pedidos.begin(); it != pedidos.end(); ++it) {
            if (it->getId() == id) {
                pedidos.erase(it);
                return;
            }
        }
        throw std::runtime_error("Pedido não encontrado para remoção");
    }

    std::vector<Pedido>& getAllPedido() {
        return pedidos;
    }

    /**
     * @brief Salva os dados do repositório em um arquivo binário de forma segura.
     */
    void salvarBinario(const char* DB_PATH) {
        std::ofstream outFile(DB_PATH, std::ios::binary);
        if (!outFile) {
            throw std::runtime_error("Erro ao abrir o arquivo para escrita");
        }

        // Salva os contadores de ID para garantir a sequência correta
        outFile.write(reinterpret_cast<const char*>(&nextLocalId), sizeof(nextLocalId));
        outFile.write(reinterpret_cast<const char*>(&nextVeiculoId), sizeof(nextVeiculoId));
        outFile.write(reinterpret_cast<const char*>(&nextPedidoId), sizeof(nextPedidoId));
        
        // Salva locais: primeiro o tamanho, depois os dados
        size_t localCount = locais.size();
        outFile.write(reinterpret_cast<const char*>(&localCount), sizeof(localCount));
        outFile.write(reinterpret_cast<const char*>(locais.data()), localCount * sizeof(Local));

        // Salva veículos: primeiro o tamanho, depois os dados
        size_t veiculoCount = veiculos.size();
        outFile.write(reinterpret_cast<const char*>(&veiculoCount), sizeof(veiculoCount));
        outFile.write(reinterpret_cast<const char*>(veiculos.data()), veiculoCount * sizeof(Veiculo));

        // Salva pedidos: primeiro o tamanho, depois os dados
        size_t pedidoCount = pedidos.size();
        outFile.write(reinterpret_cast<const char*>(&pedidoCount), sizeof(pedidoCount));
        outFile.write(reinterpret_cast<const char*>(pedidos.data()), pedidoCount * sizeof(Pedido));

        outFile.close();
    }

    /**
     * @brief Carrega os dados do repositório a partir de um arquivo binário de forma segura.
     */
    void carregarBinario(const char* DB_PATH) {
        std::ifstream inFile(DB_PATH, std::ios::binary);
        // Se o arquivo não existe (primeira execução), não faz nada.
        if (!inFile) {
            return;
        }

        // Carrega os contadores de ID para continuar a sequência
        inFile.read(reinterpret_cast<char*>(&nextLocalId), sizeof(nextLocalId));
        inFile.read(reinterpret_cast<char*>(&nextVeiculoId), sizeof(nextVeiculoId));
        inFile.read(reinterpret_cast<char*>(&nextPedidoId), sizeof(nextPedidoId));
        
        // Se o arquivo estiver vazio ou corrompido, reseta os contadores
        if(inFile.eof()) {
            nextLocalId = nextVeiculoId = nextPedidoId = 0;
            inFile.close();
            return;
        }
        // Carrega locais: primeiro o tamanho, depois os dados
        size_t localCount;
        inFile.read(reinterpret_cast<char*>(&localCount), sizeof(localCount));
        locais.resize(localCount);
        inFile.read(reinterpret_cast<char*>(locais.data()), localCount * sizeof(Local));

        // Carrega veículos: primeiro o tamanho, depois os dados
        size_t veiculoCount;
        inFile.read(reinterpret_cast<char*>(&veiculoCount), sizeof(veiculoCount));
        veiculos.resize(veiculoCount);
        inFile.read(reinterpret_cast<char*>(veiculos.data()), veiculoCount * sizeof(Veiculo));

        // Carrega pedidos: primeiro o tamanho, depois os dados
        size_t pedidoCount;
        inFile.read(reinterpret_cast<char*>(&pedidoCount), sizeof(pedidoCount));
        pedidos.resize(pedidoCount);
        inFile.read(reinterpret_cast<char*>(pedidos.data()), pedidoCount * sizeof(Pedido));

        inFile.close();
    }
};

#endif // REPOSITORIO_H
