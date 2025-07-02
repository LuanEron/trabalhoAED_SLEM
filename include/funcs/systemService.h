#ifndef SYSTEMSERVICE_H
#define SYSTEMSERVICE_H

#pragma once // Garante que o arquivo é incluído apenas uma vez
#include "classes/Local.h" // Inclui a classe Local
#include "classes/Veiculo.h"
#include "classes/Pedido.h"
#include "database/repositorio.h"
#include "funcs/veiculoService.h"
#include "funcs/pedidoService.h"
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <limits>

/**
 * @brief Salva o estado atual do repositório em um arquivo binário.
 * * @param repo Referência para o repositório de dados.
 */
void salvarDB(Repositorio& repo) {
    repo.salvarBinario("database.bin");
}

/**
 * @brief Carrega o estado do repositório a partir de um arquivo binário.
 * * @param repo Referência para o repositório de dados.
 */
void carregarDB(Repositorio& repo) {
    try {
        repo.carregarBinario("database.bin");
    } catch (const std::exception& e) {
        std::cerr << "Aviso: Não foi possível carregar a base de dados. " << e.what() << std::endl;
        std::cerr << "Iniciando com uma base de dados vazia." << std::endl;
    }
}

/**
 * @brief Calcula a distância entre dois locais.
 * * @param l1 Primeiro local.
 * @param l2 Segundo local.
 * @return double Distância entre os dois locais.
 */
double calcularDistancia(const Local& l1, const Local& l2) {
    // Implementa a lógica de cálculo de distância entre dois locais
    // Exemplo: usando a fórmula da distância euclidiana
    double distancia = sqrt(pow(l2.getCoordenadaX() - l1.getCoordenadaX(), 2) +
                            pow(l2.getCoordenadaY() - l1.getCoordenadaY(), 2));
    return distancia;
}

/**
 * @brief Seleciona o veículo disponível mais próximo da origem do pedido.
 * * @param repo Referência para o repositório de dados.
 * @param origem Local de origem do pedido.
 * @return Veiculo* Ponteiro para o veículo selecionado ou nullptr se nenhum disponível.
 */
Veiculo* selecionarVeiculo(Repositorio& repo, const Local& origem) {
    VeiculoService veiculoService(&repo);
    Veiculo* veiculoSelecionado = nullptr;
    double menorDistancia = std::numeric_limits<double>::max();

    for (auto& veiculo : veiculoService.listarVeiculos()) {
        if (veiculo.getStatus()) { // Verifica se o veículo está disponível
            double distancia = calcularDistancia(origem, veiculo.getLocal());
            if (distancia < menorDistancia) {
                menorDistancia = distancia;
                veiculoSelecionado = &veiculo;
            }
        }
    }

    if (veiculoSelecionado) {
        veiculoSelecionado->setStatus(false);
    } else {
        std::cout << "Nenhum veículo disponível encontrado." << std::endl;
    }
    return veiculoSelecionado;
}

/**
 * @brief Calcula a distância total da rota de entrega de um pedido.
 * * @param veiculoSelecionado Veículo que fará a entrega.
 * @param pedido Pedido a ser entregue.
 * @return double Distância total (origem do veículo até origem do pedido + origem até destino).
 */
double calcularRota(const Veiculo& veiculoSelecionado, const Pedido& pedido) {
    double distanciaOrigem = calcularDistancia(veiculoSelecionado.getLocal(), pedido.getOrigem());
    double distanciaDestino = calcularDistancia(pedido.getOrigem(), pedido.getDestino());
    double distanciaTotal = distanciaOrigem + distanciaDestino;
    return distanciaTotal;
}

/**
 * @brief Finaliza um pedido, atualizando o status do pedido e do veículo, e local atual do veiculo.
 * * @param repo Referência para o repositório de dados.
 * @param pedido Referência para o pedido a ser finalizado.
 * @param veiculo Referência para o veículo que realizou a entrega.
 */
void finalizarPedido(Repositorio& repo, Pedido& pedido, Veiculo& veiculo) {
    veiculo.setStatus(true);// Marca o veículo como disponível
    veiculo.setLocal(pedido.getDestino()); // Atualiza a localização do veículo para o destino do pedido
    pedido.setStatus(true); // Marca o pedido como concluído
}

/**
 * @brief Realiza o processo de entrega de um pedido, selecionando o veículo, calculando a rota e finalizando o pedido.
 * * @param repo Referência para o repositório de dados.
 * @param pedido Referência para o pedido a ser entregue.
 */
void realizarEntrega(Repositorio& repo, Pedido& pedido) {
    Veiculo* pVeiculoSelecionado = selecionarVeiculo(repo, pedido.getOrigem());

    if (!pVeiculoSelecionado) {
        std::cout << "Não foi possível realizar a entrega, nenhum veículo pode ser selecionado." << std::endl;
        return;
    }

    Veiculo& veiculoSelecionado = *pVeiculoSelecionado;
    
    double distanciaTotal = calcularRota(veiculoSelecionado, pedido);
    std::cout << "Veículo selecionado: " << veiculoSelecionado.getModelo() 
              << " (Placa: " << veiculoSelecionado.getPlaca() << ")" << std::endl;
    std::cout << "ID do pedido: " << pedido.getId() << std::endl;
    std::cout << "Detalhes do pedido:" << std::endl;
    std::cout << "Origem: " << pedido.getOrigem().getEndereco().getRua() 
              << ", Destino: " << pedido.getDestino().getEndereco().getRua() << std::endl;
    std::cout << "Peso do pedido: " << pedido.getPeso() << " Kg" << std::endl;
    std::cout << "Distância do veículo até a origem: " 
              << calcularDistancia(veiculoSelecionado.getLocal(), pedido.getOrigem()) << " Km" << std::endl;
    std::cout << "Distância da origem até o destino: " 
              << calcularDistancia(pedido.getOrigem(), pedido.getDestino()) << " Km" << std::endl;
    std::cout << "Distância total da rota: " << distanciaTotal << " Km" << std::endl;

    // Simula a entrega
    std::cout << "\nEntregando o pedido..." << std::endl;
    finalizarPedido(repo, pedido, veiculoSelecionado);
    std::cout << "Pedido entregue com sucesso!" << std::endl;
}

/**
 * @brief Exibe na tela os contadores e estatísticas gerais do sistema.
 * @param repo Referência para o repositório de dados.
 * @details Calcula e mostra o número total de locais, veículos, e pedidos (separados por status).
 * A contagem é feita em tempo real, garantindo que os dados estejam sempre atualizados.
 * Complexidade: O(P), onde P é o número de pedidos (para contar os status).
 */
void exibirEstatisticas(Repositorio& repo) {
    PedidoService pedidoService(&repo);
    int pedidosPendentes = 0;
    int pedidosEntregues = 0;

    // Contagem de pedidos por status
    for (const auto& pedido : pedidoService.listarPedidos()) {
        if (pedido.getStatus()) {
            pedidosEntregues++;
        } else {
            pedidosPendentes++;
        }
    }

    std::cout << "\n--- Estatísticas do Sistema (Contadores) ---\n" << std::endl;
    std::cout << "Total de Locais cadastrados: " << repo.getAllLocal().size() << std::endl;
    std::cout << "Total de Veículos cadastrados: " << repo.getAllVeiculo().size() << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "Total de Pedidos: " << repo.getAllPedido().size() << std::endl;
    std::cout << "  - Pedidos Pendentes: " << pedidosPendentes << std::endl;
    std::cout << "  - Pedidos Entregues: " << pedidosEntregues << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
}

#endif // SYSTEMSERVICE_H
