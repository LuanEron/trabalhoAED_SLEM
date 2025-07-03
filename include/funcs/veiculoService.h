#ifndef FUNC_veiculoService_H
#define FUNC_veiculoService_H

#include <iostream>
#include <vector>
#include "classes/Endereco.h"
#include "classes/Local.h"
#include "classes/Veiculo.h"
#include "funcs/localService.h"
#include "database/repositorio.h" 

/**
 * @class VeiculoService
 * @brief Classe responsável por gerenciar as operações CRUD para veículos.
 * * Esta classe fornece métodos para criar, ler, atualizar e deletar veículos,
 * utilizando o repositório de dados.
 */
class VeiculoService {
    private:
        Repositorio* repositorio; // Ponteiro para o repositório onde os veículos são armazenados
    public:
        VeiculoService() = default; // Construtor padrão
        VeiculoService(Repositorio* repo) : repositorio(repo) {} // Construtor que recebe um repositório
        
        // ================== CRUD PARA VEÍCULOS ==================
        /**
         * @brief Cria um novo veículo com a placa, modelo, local e status fornecidos.
         * * @param placa Placa do veículo a ser criado.
         * @param modelo Modelo do veículo a ser criado.
         * @param local Local onde o veículo está estacionado.
         * @param status Status do veículo (true = disponível, false = indisponível).
         * @return int ID do veículo criado, ou -1 se a placa for inválida.
         */
        int criarVeiculo(const char* placa, const char* modelo, const Local& local, bool status) {
            Veiculo novo(placa, modelo, local, status);
            if(!novo.setPlaca(placa)) return -1; // Retorna -1 se a placa for inválida
            if(!novo.setModelo(modelo)) return -1;
            if(!novo.setLocal(local)) return -1; // Retorna -1 se o local for inválido
            novo.setStatus(true); // Define o status como true por padrão
            return repositorio->addVeiculo(novo);
        }

        /**
         * @brief Lê um veículo pelo placa.
         * * @param placa Placa do veículo a ser lido
         * @return Veiculo* Ponteiro para o objeto Veiculo, ou nullptr se não encontrado.
         */
        Veiculo* lerVeiculo(const char* placa) {
            return repositorio->getVeiculo(placa);
        }

        /**
         * @brief Atualiza um veículo existente com os novos dados fornecidos.
         * * @param placa Placa do veículo a ser atualizado.
         * @param modelo Novo modelo do veículo.
         * @param local Novo local onde o veículo está estacionado.
         * @param status Novo status do veículo (true = disponível, false = indisponível).
         * @return true se a atualização for bem-sucedida, false caso contrário.
         */
        bool atualizarVeiculo(const char* placa, const char* modelo, const Local& local, bool status) {
            Veiculo* veiculo = repositorio->getVeiculo(placa);
            if (!veiculo) return false; // Veículo não encontrado

            veiculo->setModelo(modelo);
            veiculo->setLocal(local);
            veiculo->setStatus(status);
            repositorio->updateVeiculo(*veiculo);
            return true;
        }

        /**
         * @brief Deleta um veículo pelo placa.
         * * @param placa Placa do veículo a ser deletado.
         * @return true se a deleção for bem-sucedida, false caso contrário.
         */
        bool deletarVeiculo(const char* placa) {
            try {
                repositorio->removeVeiculo(placa);
                return true;
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro ao deletar veículo: " << e.what() << std::endl;
                return false;
            }
        }
};

/**
 * @brief Função para adicionar um novo veículo ao repositório.
 * @param repo Ponteiro para o repositório onde o veículo será adicionado.
 * Esta função solicita ao usuário as informações do veículo, como placa, modelo e local,
 * e utiliza a classe VeiculoService para criar o veículo e adicioná-lo ao repositório.
 */
void adicionarVeiculo(Repositorio* repo) {
    VeiculoService veiculoService(repo);
    char placa[8], modelo[50];
    Local local;
    int localId;

    std::cout << "Digite a placa do veículo (formato AAA1234): ";
    std::cin >> placa;
    std::cin.ignore(); 
    std::cout << "Digite o modelo do veículo: ";
    std::cin.getline(modelo, 50);

    listarLocais(repo);
    std::cout << "Digite o ID do local onde o veículo está estacionado: ";
    std::cin >> localId;
    Local* pLocal = repo->getLocal(localId);
    if (!pLocal) {
        std::cerr << "Erro: Local com ID " << localId << " não encontrado." << std::endl;
        return;
    }
    local = *pLocal;

    int id = veiculoService.criarVeiculo(placa, modelo, local, true);
    if (id != -1) {
        std::cout << "Veículo adicionado com sucesso! ID: " << id << std::endl;
    } else {
        std::cerr << "Erro ao adicionar veículo. Verifique a placa e os dados." << std::endl;
    }
}

/**
 * @brief Função para listar todos os veículos no repositório.
 * @param repo Ponteiro para o repositório de onde os veículos serão listados.
 * Esta função obtém todos os veículos do repositório e os exibe no console.
 */
void listarVeiculos(Repositorio* repo) {
    std::cout << "Lista de veículos:" << std::endl;
    Veiculo* veiculos = repo->getAllVeiculo();
    int numVeiculos = repo->getNumVeiculos();

    if (numVeiculos == 0) {
        std::cout << "Nenhum veículo cadastrado." << std::endl;
        return;
    }

    for (int i = 0; i < numVeiculos; ++i) {
        const auto& veiculo = veiculos[i];
        std::cout << "ID: " << veiculo.getId() 
                  << ", Placa: " << veiculo.getPlaca() 
                  << ", Modelo: " << veiculo.getModelo() 
                  << ", Local: " << veiculo.getLocal().getEndereco().getRua() 
                  << ", Status: " << (veiculo.getStatus() ? "Disponivel" : "Indisponivel") 
                  << std::endl;
    }
}

/**
 * @brief Função para editar um veículo existente no repositório.
 * @param repo Ponteiro para o repositório onde o veículo será editado.
 * Esta função solicita ao usuário a placa do veículo a ser editado e as novas informações do veículo,
 * e utiliza a classe VeiculoService para atualizar o veículo no repositório.
 */
void editarVeiculo(Repositorio* repo) {
    VeiculoService veiculoService(repo);
    char placa[8], novoModelo[50];
    Local novoLocal;
    bool novoStatus;

    std::cout << "Digite a placa do veículo a ser editado: ";
    std::cin >> placa;
    std::cin.ignore();

    Veiculo* veiculo = veiculoService.lerVeiculo(placa);
    if (!veiculo) {
        std::cerr << "Veículo não encontrado." << std::endl;
        return;
    }

    std::cout << "Digite o novo modelo do veículo: ";
    std::cin.getline(novoModelo, 50);

    listarLocais(repo);
    std::cout << "Digite o ID do novo local onde o veículo está estacionado: ";
    int localId;
    std::cin >> localId;
    Local* pLocal = repo->getLocal(localId);
    if(!pLocal){
        std::cerr << "Local não encontrado." << std::endl;
        return;
    }
    novoLocal = *pLocal;

    std::cout << "O veículo está disponível? (1 para Sim, 0 para Não): ";
    int statusInput;
    std::cin >> statusInput;
    novoStatus = (statusInput == 1);

    if (veiculoService.atualizarVeiculo(placa, novoModelo, novoLocal, novoStatus)) {
        std::cout << "Veículo atualizado com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro ao atualizar veículo." << std::endl;
    }
}

/**
 * @brief Função para remover um veículo do repositório.
 * @param repo Ponteiro para o repositório de onde o veículo será removido. 
 * Esta função solicita ao usuário a placa do veículo a ser removido
 * e utiliza a classe VeiculoService para deletar o veículo do repositório.
 */
void removerVeiculo(Repositorio* repo) {
    VeiculoService veiculoService(repo);
    char placa[8];

    std::cout << "Digite a placa do veículo a ser removido: ";
    std::cin >> placa;

    if (veiculoService.deletarVeiculo(placa)) {
        std::cout << "Veículo removido com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro ao remover veículo." << std::endl;
    }
}

#endif // FUNC_veiculoService_H
