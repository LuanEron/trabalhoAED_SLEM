#ifndef FUNC_localService_H
#define FUNC_localService_H

#include <iostream>
#include <limits>
#include "classes/Endereco.h"
#include "classes/Local.h"
#include "database/repositorio.h"

class LocalService {
    private:
        Repositorio* repositorio;
    public:
        LocalService() = default;
        LocalService(Repositorio* repo) : repositorio(repo) {}
        
        // ================== CRUD PARA LOCAIS ==================
        int criarLocal(const Endereco& endereco, double x, double y) {
            Local novo(endereco, x, y);
            novo.setEndereco(endereco);
            if(!novo.setCoordenadaX(x))
                return -1; // Retorna -1 se coordenada X for inválida
            if(!novo.setCoordenadaY(y))
                return -1; // Retorna -1 se coordenada Y for inválida
            return repositorio->addLocal(novo);
        }

        Local* lerLocal(int id) {
            return repositorio->getLocal(id);
        }

        bool atualizarLocal(int id, const Endereco& novoEndereco, double novoX, double novoY) {
            Local* local = repositorio->getLocal(id);
            if (!local) return false; // Local não encontrado

            local->setEndereco(novoEndereco);
            if (!local->setCoordenadaX(novoX)) {
                return false;
            }
            if (!local->setCoordenadaY(novoY)) {
                return false;
            }
            repositorio->updateLocal(*local);
            return true;
        }

        bool deletarLocal(int id) {
            try {
                repositorio->removeLocal(id);
                return true;
            } catch (const std::runtime_error& e) {
                std::cerr << "Erro ao deletar local: " << e.what() << std::endl;
                return false;
            }
        }
};

 void adicionarLocal(Repositorio* repo) {
    char rua[50], cidade[50], estado[3];
    double x, y;
    Endereco endereco;
    Local local;
        std::cout << "Digite o endereço do local (rua, cidade, estado)" << std::endl;
        std::cin.ignore();
            
        std::cout << "Digite a rua: "<< std::endl;
        std::cin.getline(rua, 50);
        endereco.setRua(rua);
            
        std::cout << "Digite a cidade: "<< std::endl;
        std::cin.getline(cidade, 50);
        endereco.setCidade(cidade);
            
        std::cout << "Digite o estado (sigla de 2 letras): " << std::endl;
        std::cin.getline(estado, 3);
        endereco.setEstado(estado);
        
        std::cout << "Digite as coordenadas (x, y) [Separadas apenas por espaço]: ";
        std::cin >> x >> y;
        local.setCoordenadaX(x);
        local.setCoordenadaY(y);
            
        LocalService localService(repo);
        int id = localService.criarLocal(endereco, local.getCoordenadaX(), local.getCoordenadaY());
        if (id != -1) {
            std::cout << "Local criado com ID: " << id << std::endl;
        } else {
            std::cerr << "Erro ao criar local." << std::endl;
            }
    }

    void listarLocais(Repositorio* repo) {
       std::cout << "Lista de Locais:" << std::endl;
        Local* locais = repo->getAllLocal();
        int numLocais = repo->getNumLocais();

        if (numLocais == 0) {
            std::cout << "Nenhum local cadastrado." << std::endl;
            return;
        }

        for (int i = 0; i < numLocais; ++i) {
            const auto& local = locais[i];
            std::cout << "ID: " << local.getId() 
                      << ", Endereço: " << local.getEndereco().getRua() 
                      << ", Coordenadas: (" << local.getCoordenadaX() 
                      << ", " << local.getCoordenadaY() << ")" << std::endl;
        }
    }

    void editarLocal(Repositorio* repo) {
        int id;
        char rua[50], cidade[50], estado[3];
        double x, y;
        Endereco novoEndereco;
        LocalService localService(repo);

        std::cout << "Digite o ID do local a ser editado: ";
        std::cin >> id;

        Local* local = localService.lerLocal(id);
        if (!local) {
            std::cerr << "Local não encontrado." << std::endl;
            return;
        }

        std::cout << "Digite o novo endereço do local (rua, cidade, estado)";
        std::cin.ignore();
        
        std::cout << "Digite a rua: " << std::endl;
        std::cin.getline(rua, 50);
        novoEndereco.setRua(rua);
        
        std::cout << "Digite a cidade: "<< std::endl;
        std::cin.getline(cidade, 50);
        novoEndereco.setCidade(cidade);
        
        std::cout << "Digite o estado (sigla de 2 letras): " << std::endl;
        std::cin.getline(estado, 3);
        novoEndereco.setEstado(estado);

        std::cout << "Digite as novas coordenadas (x, y) [Separadas apenas por espaço]: ";
        std::cin >> x >> y;

        if (localService.atualizarLocal(id, novoEndereco, x, y)) {
            std::cout << "Local com ID " << id << " atualizado com sucesso." << std::endl;
        } else {
            std::cerr << "Erro ao atualizar local com ID " << id << "." << std::endl;
        }
    }

    void removerLocal(Repositorio* repo) {
        int id;
        LocalService localService(repo);

        std::cout << "Digite o ID do local a ser removido: ";
        std::cin >> id;

        if (localService.deletarLocal(id)) {
            std::cout << "Local com ID " << id << " removido com sucesso." << std::endl;
        } else {
            std::cerr << "Erro ao remover local com ID " << id << "." << std::endl;
        }
    }

#endif // FUNC_localService_H
