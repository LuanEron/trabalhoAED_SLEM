#ifndef FUNC_veiculoService_H
#define FUNC_veiculoService_H

#include <iostream>
#include <vector>
#include "classes/Endereco.h"
#include "classes/Local.h"
#include "classes/Veiculo.h"
#include "funcs/localService.h"
#include "database/repositorio.h" 

class VeiculoService {
    private:
        Repositorio* repositorio;
    public:
        VeiculoService() = default;
        VeiculoService(Repositorio* repo) : repositorio(repo) {}
        
        // ================== CRUD PARA VEÍCULOS ==================
        int criarVeiculo(const char* placa, const char* modelo, const Local& local, bool status) {
            Veiculo novo(placa, modelo, local, status);
            if(!novo.setPlaca(placa)) return -1; // Retorna -1 se a placa for inválida
            if(!novo.setModelo(modelo)) return -1; // Retorna -1 se
            if(!novo.setLocal(local)) return -1; // Retorna -1 se o local for inválido
            novo.setStatus(true); // Define o status como true por padrão
            return repositorio->addVeiculo(novo);
        }

        Veiculo* lerVeiculo(const char* placa) {
            return repositorio->getVeiculo(placa);
        }

        bool atualizarVeiculo(const char* placa, const char* modelo, const Local& local, bool status) {
            Veiculo* veiculo = repositorio->getVeiculo(placa);
            if (!veiculo) return false; // Veículo não encontrado

            veiculo->setModelo(modelo);
            veiculo->setLocal(local);
            veiculo->setStatus(status);
            repositorio->updateVeiculo(*veiculo);
            return true;
        }

        bool deletarVeiculo(const char* placa) {
            try {
                repositorio->removeVeiculo(placa);
                return true;
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro ao deletar veículo: " << e.what() << std::endl;
                return false;
            }
        }

        std::vector<Veiculo>& listarVeiculos() {
            return repositorio->getAllVeiculo();
        }
};

void adicionarVeiculo(Repositorio* repo) {
    VeiculoService veiculoService(repo);
    char placa[8], modelo[50];
    Local local;
    bool status;
    int localId;

    std::cout << "Digite a placa do veículo (formato AAA0000): ";
    std::cin >> placa;
    std::cin.ignore(); // Limpa o buffer do cin para evitar problemas com getline
    std::cout << "Digite o modelo do veículo: ";
    std::cin.getline(modelo, 50);

    // associa a um local salvo
    listarLocais(repo);
    std::cout << "Digite o ID do local onde o veículo está estacionado: ";
    std::cin >> localId;
    local = *repo->getLocal(localId);
    

    status = true; // Defina o status como true por padrão

    int id = veiculoService.criarVeiculo(placa, modelo, local, status);
    if (id != -1) {
        std::cout << "Veículo adicionado com sucesso! ID: " << id << std::endl;
    } else {
        std::cerr << "Erro ao adicionar veículo." << std::endl;
    }
}

void listarVeiculos(Repositorio* repo) {
    VeiculoService veiculoService(repo);
    std::cout << "Lista de veículos:" << std::endl;
    for (const auto& veiculo : veiculoService.listarVeiculos()) {
        std::cout << "ID: " << veiculo.getId() 
                  << ", Placa: " << veiculo.getPlaca() 
                  << ", Modelo: " << veiculo.getModelo() 
                  << ", Local: " << veiculo.getLocal().getEndereco().getRua() 
                                 << veiculo.getLocal().getEndereco().getCidade()
                                 << veiculo.getLocal().getEndereco().getEstado()
                  << ", Coordenadas: (" << veiculo.getLocal().getCoordenadaX() << veiculo.getLocal().getCoordenadaY()
                      << ", Status: " << (veiculo.getStatus() ? "Disponivel" : "Indisponivel") 
                      << std::endl;
        }
    }

void editarVeiculo(Repositorio* repo) {
    VeiculoService veiculoService(repo);
    char placa[8], modelo[50];
    Local local;
    bool status;

    std::cout << "Digite a placa do veículo a ser editado: ";
    std::cin >> placa;

    Veiculo* veiculo = veiculoService.lerVeiculo(placa);
    if (!veiculo) {
        std::cerr << "Veículo não encontrado." << std::endl;
        return;
    }

    std::cout << "Digite o novo modelo do veículo: ";
    std::cin.getline(modelo, 50);

    // associa a um local salvo
    listarLocais(repo);
    std::cout << "Digite o ID do novo local onde o veículo está estacionado: ";
    int localId;

    status = true; // Defina o status como true por padrão

    if (veiculoService.atualizarVeiculo(placa, modelo, local, status)) {
        std::cout << "Veículo atualizado com sucesso!" << std::endl;
    } else {
        std::cerr << "Erro ao atualizar veículo." << std::endl;
    }
}

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
