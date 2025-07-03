#ifndef SYSTEMSERVICE_H
#define SYSTEMSERVICE_H

#pragma once 
#include "classes/Local.h" 
#include "classes/Veiculo.h"
#include "classes/Pedido.h"
#include "database/repositorio.h"
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <limits>

/**
 * @brief Salva o estado atual do repositório em um arquivo binário.
 * * @param repo Referência para o repositório de dados.
 * @details Esta função salva todos os dados do repositório, incluindo locais, veículos e pedidos,
 */
void salvarDB(Repositorio& repo) {
    repo.salvarBinario("include/database/database.bin");
}

/**
 * @brief Carrega o estado do repositório a partir de um arquivo binário.
 * * @param repo Referência para o repositório de dados.
 * @details Esta função carrega os dados do repositório a partir de um arquivo binário previamente salvo.
 * Ela deve ser chamada no início do programa para garantir que os dados persistam entre as execuções.
 */
void carregarDB(Repositorio& repo) {
    repo.carregarBinario("include/database/database.bin");
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
Veiculo * selecionarVeiculo(Repositorio& repo, const Local& origem) {
    Veiculo* veiculos = repo.getAllVeiculo();
    int numVeiculos = repo.getNumVeiculos();
    int indiceVeiculoSelecionado = -1;
    double menorDistancia = std::numeric_limits<double>::max();

    for (int i = 0; i < numVeiculos; ++i) {
        if (veiculos[i].getStatus()) { // Verifica se o veículo está disponível
            double distancia = calcularDistancia(origem, veiculos[i].getLocal());
            if (distancia == 0) {
                indiceVeiculoSelecionado = i;
                break; // Não precisa procurar mais, já encontrou o veículo mais próximo
            }
            if (distancia < menorDistancia) {
                menorDistancia = distancia;
                indiceVeiculoSelecionado = i;
            }
        }
    }

    if (indiceVeiculoSelecionado != -1) {
        Veiculo* veiculoSelecionado = &veiculos[indiceVeiculoSelecionado];
        veiculoSelecionado->setStatus(false); // Marca o veículo como ocupado
        repo.updateVeiculo(*veiculoSelecionado);
        return veiculoSelecionado;
    } else {
        std::cout << "Nenhum veículo disponível encontrado." << std::endl;
        return nullptr;
    }
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
    repo.updatePedido(pedido);
    repo.updateVeiculo(veiculo);
}

/**
 * @brief Realiza o processo de entrega de um pedido, selecionando o veículo, calculando a rota e finalizando o pedido.
 * * @param repo Referência para o repositório de dados.
 * @param pedido Referência para o pedido a ser entregue.
 */
void realizarEntrega(Repositorio& repo, Pedido& pedido) {
    Veiculo* pVeiculoSelecionado = selecionarVeiculo(repo, pedido.getOrigem());

    if (!pVeiculoSelecionado) {
        // A mensagem de erro já é exibida em selecionarVeiculo
        return;
    }

    // O ponteiro é dereferenciado para uma cópia para exibição e cálculos seguros.
    Veiculo veiculoCopia = *pVeiculoSelecionado;
    double distanciaTotal = calcularRota(veiculoCopia, pedido);

    std::cout << "Veículo selecionado: " << veiculoCopia.getModelo() 
              << " (Placa: " << veiculoCopia.getPlaca() << ")" << std::endl;
    std::cout << "ID do pedido: " << pedido.getId() << std::endl;
    std::cout << "Detalhes do pedido:" << std::endl;
    std::cout << "Origem: " << pedido.getOrigem().getEndereco().getRua() 
              << ", Destino: " << pedido.getDestino().getEndereco().getRua() << std::endl;
    std::cout << "Peso do pedido: " << pedido.getPeso() << " Kg" << std::endl;
    std::cout << "Distância do veículo até a origem: " 
              << calcularDistancia(veiculoCopia.getLocal(), pedido.getOrigem()) << " Km" << std::endl;
    std::cout << "Distância da origem até o destino: " 
              << calcularDistancia(pedido.getOrigem(), pedido.getDestino()) << " Km" << std::endl;
    std::cout << "Distância total da rota: " << distanciaTotal << " Km" << std::endl;

    // Simula a entrega
    std::cout << "Entregando o pedido..." << std::endl;
    // A função finalizarPedido modifica o veículo original, então passamos o ponteiro dereferenciado.
    finalizarPedido(repo, pedido, *pVeiculoSelecionado);
    std::cout << "Pedido entregue com sucesso!" << std::endl;
}

#endif // SYSTEMSERVICE_H
