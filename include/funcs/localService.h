#ifndef FUNC_localService_H
#define FUNC_localService_H

#include <iostream>
#include <vector>
#include "classes/Endereco.h"
#include "classes/Local.h"
#include "database/repositorio.h"
#include <limits>


/**
 * @class LocalService
 * @brief Classe responsável por gerenciar as operações CRUD para locais.
 * * Esta classe fornece métodos para criar, ler, atualizar e deletar locais,
 * utilizando o repositório de dados.
 */
class LocalService {
    private:
        Repositorio* repositorio; // Ponteiro para o repositório onde os locais são armazenados
    public:
        LocalService() = default; // Construtor padrão
        LocalService(Repositorio* repo) : repositorio(repo) {} // Construtor que recebe um repositório
        
        // ================== CRUD PARA LOCAIS ==================
        /**
         * @brief Cria um novo local com o endereço e coordenadas fornecidos.
         * * @param endereco Endereço do local a ser criado.
         * @param x Coordenada X do local.
         * @param y Coordenada Y do local.
         * @return int ID do local criado, ou -1 se as coordenadas forem inválidas.
         */
        int criarLocal(const Endereco& endereco, double x, double y) {
            Local novo(endereco, x, y);
            novo.setEndereco(endereco);
            if(!novo.setCoordenadaX(x))
                return -1; // Retorna -1 se coordenada X for inválida
            if(!novo.setCoordenadaY(y))
                return -1; // Retorna -1 se coordenada Y for inválida
            return repositorio->addLocal(novo);
        }

        /**
         * @brief Lê um local pelo ID.
         * * @param id ID do local a ser lido.
         * @return Local* Ponteiro para o objeto Local, ou nullptr se não encontrado.
         */
        Local* lerLocal(int id) {
            return repositorio->getLocal(id);
        }

        /**
         * @brief Atualiza um local existente com o novo endereço e coordenadas.
         * * @param id ID do local a ser atualizado.
         * @param novoEndereco Novo endereço do local.
         * @param novoX Nova coordenada X do local.
         * @param novoY Nova coordenada Y do local.
         * @return true se a atualização for bem-sucedida, false caso contrário.
         */
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

        /**
         * @brief Deleta um local pelo ID.
         * * @param id ID do local a ser deletado.
         * @return true se a deleção for bem-sucedida, false caso contrário.
         */
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

/**
 * @brief Função para adicionar um novo local ao repositório.
 * @param repo Ponteiro para o repositório onde o local será adicionado.
 * Esta função solicita ao usuário as informações do local, como endereço e coordenadas,
 * e utiliza a classe LocalService para criar o local e adicioná-lo ao repositório.
 */
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

/**
 * @brief Função para listar todos os locais no repositório.
 * @param repo Ponteiro para o repositório de onde os locais serão listados.
 * Esta função obtém todos os locais do repositório e os exibe no console.
 */
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

/**
 * @brief Função para editar um local existente no repositório.
 * @param repo Ponteiro para o repositório onde o local será editado.
 * Esta função solicita ao usuário o ID do local a ser editado e as novas informações do local,
 * e utiliza a classe LocalService para atualizar o local no repositório.
 */
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

    std::cout << "Digite o novo endereço do local (rua, cidade, estado)" << std::endl;
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

/**
 * @brief Função para remover um local do repositório.
 * @param repo Ponteiro para o repositório de onde o local será removido. 
 * Esta função solicita ao usuário o ID do local a ser removido
 * e utiliza a classe LocalService para deletar o local do repositório.
 * @throws std::runtime_error Se o local não for encontrado.
 */
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

#endif // FUNC_localServices_H
