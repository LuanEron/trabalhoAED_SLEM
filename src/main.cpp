#include <iostream> // Biblioteca para entrada e saída de dados
#include <vector> // Biblioteca para uso de vetores
#include <cmath> // Biblioteca para funções matemáticas
#include <limits> // Biblioteca para limites de tipos numéricos
#include <stdbool.h> // Biblioteca para uso de booleanos
#include "classes/Endereco.h" // Inclui a classe Endereco
#include "classes/Local.h" // Inclui a classe Local
#include "classes/Veiculo.h" // Inclui a classe Veiculo
#include "classes/Pedido.h" // Inclui a classe Pedido
#include "database/repositorio.h" // Inclui a classe Repositorio
#include "funcs/localService.h" // Inclui o serviço de manipulação de locais
#include "funcs/veiculoService.h" // Inclui o serviço de manipulação de veículos
#include "funcs/pedidoService.h" // Inclui o serviço de manipulação de pedidos
#include "funcs/systemService.h" // Inclui o serviço de manipulação do sistema
#include <locale.h> // Biblioteca para manipulação de locale

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Define o locale para português brasileiro
    // Inicializa o repositório
    Repositorio repo;
    carregarDB(repo);

    int op = -1;
    int entregaOp = -1;
    int veiculoOp = -1;
    int pedidoOp = -1;
    int localOp =-1;
    do {
    std::cout << "Bem-vindo ao SLEM!" << std::endl;
    std::cout << "Selecione uma opção:" << std::endl;
    std::cout << "1. Locais" << std::endl;
    std::cout << "2. Veículos" << std::endl;
    std::cout << "3. Pedidos" << std::endl;
    std::cout << "4. Exibir Estatísticas (Contadores)" << std::endl; 
    std::cout << "5. Gerenciar Entregas" << std::endl;       
    std::cout << "0. Sair" << std::endl;
    std::cin >> op;
    std::cin.ignore(); // Limpa o buffer do cin para evitar problemas com getline
    system("cls"); // Limpa a tela do console
    if (op < 0 || op > 5) 
        std::cout << "Opção inválida. Tente novamente." << std::endl;
    switch (op) {
        case 1:
            do {
                std::cout << "Opções Locais." << std::endl;
                std::cout << "1. Adicionar Local" << std::endl;
                std::cout << "2. Listar Locais" << std::endl;
                std::cout << "3. Editar Local" << std::endl;
                std::cout << "4. Remover Local" << std::endl;
                std::cout << "0. Voltar" << std::endl;
                std::cout << "Digite a opção desejada: ";

                std::cin >> localOp;
                system("cls"); // Limpa a tela do console
                switch (localOp) {
                    case 1:
                        adicionarLocal(&repo);
                        if (std::cin.fail()) {
                            std::cin.clear(); // limpa o estado de erro
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta o restante da linha
                            localOp = -1; // força opção inválida
                        }
                        localOp = -1;
                        break;
                    case 2:
                        listarLocais(&repo);
                        localOp = -1;
                        break;
                    case 3:
                        editarLocal(&repo);
                        if (std::cin.fail()) {
                            std::cin.clear(); // limpa o estado de erro
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta o restante da linha
                            localOp = -1; // força opção inválida
                        }
                        localOp = -1;
                        break;
                    case 4:
                        removerLocal(&repo);
                        localOp = -1;
                        break;
                    case 0:
                        std::cout << "Voltando ao menu principal." << std::endl;
                        system("cls"); // Limpa a tela do console
                        break;

                    default:
                        std::cout << "Opção inválida." << std::endl;
                        system("cls"); // Limpa a tela do console
                        break;
                }
        } while (localOp != 0);
            break;
        case 2:
            do {
                std::cout << "Opções Veículos." << std::endl;
                std::cout << "1. Adicionar Veículo" << std::endl;
                std::cout << "2. Listar Veículos" << std::endl;
                std::cout << "3. Editar Veículo" << std::endl;
                std::cout << "4. Remover Veículo" << std::endl;
                std::cout << "0. Voltar" << std::endl;
                std::cin >> veiculoOp;
                system("cls"); // Limpa a tela do console
                switch (veiculoOp) {
                    case 1:
                        adicionarVeiculo(&repo);
                        if (std::cin.fail()) {
                            std::cin.clear(); // limpa o estado de erro
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta o restante da linha
                            veiculoOp = -1; // força opção inválida
                        }
                        veiculoOp = -1;
                        break;
                    case 2:
                        listarVeiculos(&repo);
                        veiculoOp = -1;
                        break;
                    case 3:
                        editarVeiculo(&repo);
                        if (std::cin.fail()) {
                            std::cin.clear(); // limpa o estado de erro
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta o restante da linha
                            veiculoOp = -1; // força opção inválida
                        }
                        veiculoOp = -1;
                        break;
                    case 4:
                        removerVeiculo(&repo);
                        if (std::cin.fail()) {
                            std::cin.clear(); // limpa o estado de erro
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta o restante da linha
                            veiculoOp = -1; // força opção inválida
                        }
                        veiculoOp = -1;
                        break;
                    case 0:
                        std::cout << "Voltando ao menu principal." << std::endl;
                        system("cls"); // Limpa a tela do console
                        break;
                    default:
                        std::cout << "Opção inválida." << std::endl;
                        system("cls"); // Limpa a tela do console
                }
            } while (veiculoOp != 0);
            break;
        case 3:
            do {
                std::cout << "Opções Pedidos." << std::endl;
                std::cout << "1. Adicionar Pedido" << std::endl;
                std::cout << "2. Listar Pedidos" << std::endl;
                std::cout << "3. Editar Pedido" << std::endl;
                std::cout << "4. Remover Pedido" << std::endl;
                std::cout << "0. Voltar" << std::endl;
                
                std::cin >> pedidoOp;
                system("cls"); // Limpa a tela do console
                switch (pedidoOp) {
                    case 1:
                        adicionarPedido(&repo);
                        if (std::cin.fail()) {
                        std::cin.clear(); // limpa o estado de erro
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta o restante da linha
                        pedidoOp = -1; // força opção inválida
                        }
                        pedidoOp = -1;
                        break;
                    case 2:
                        listarPedidos(&repo);
                        pedidoOp = -1;
                        break;
                    case 3:
                        editarPedido(&repo);
                        if (std::cin.fail()) {
                            std::cin.clear(); // limpa o estado de erro
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta o restante da linha
                            pedidoOp = -1; // força opção inválida
                        }
                        pedidoOp = -1;
                        break;
                    case 4:
                        removerPedido(&repo);
                        if (std::cin.fail()) {
                            std::cin.clear(); // limpa o estado de erro
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // descarta o restante da linha
                            pedidoOp = -1; // força opção inválida
                        }
                        pedidoOp = -1;
                        break;
                    case 0:
                        std::cout << "Voltando ao menu principal." << std::endl;
                        system("cls"); // Limpa a tela do console
                        break;
                    default:
                        std::cout << "Opção inválida." << std::endl;
                        system("cls"); // Limpa a tela do console
                }
            } while (pedidoOp != 0);
            break;    
        case 4:
        {
            exibirEstatisticas(repo);
            // Pausa para o usuário poder ler as estatísticas
            std::cout << "\nPressione Enter para continuar...";
            std::cin.get(); 
            break;
        }       
        case 5:
            do {
                std::cout << "Opções Gerenciar Entregas." << std::endl;
                std::cout << "1. Listar Pedidos Pendentes" << std::endl;
                std::cout << "2. Listar Pedidos Entregues" << std::endl;
                std::cout << "3. Realizar Nova Entrega" << std::endl;
                std::cout << "0. Voltar" << std::endl;
                std::cin >> entregaOp;
                system("cls"); // Limpa a tela do console
                switch (entregaOp) {
                    case 1:
                        listarPedidosPendentes(&repo);
                        entregaOp = -1;
                        break;
                    case 2:
                        listarPedidosEntregues(&repo);
                        entregaOp = -1;
                        break;
                    case 3: {
                        std::cout << "Digite o ID do pedido a ser entregue: ";
                        int pedidoId;
                        std::cin >> pedidoId;
                        std::cin.ignore(); // Limpa o buffer do cin para evitar problemas com getline
                        Pedido* pedido = repo.getPedido(pedidoId);
                        if (!pedido) {
                            std::cerr << "Pedido não encontrado." << std::endl;
                            break;
                        }
                        realizarEntrega(repo, *pedido);
                        entregaOp = -1;
                        break;
                    }
                    case 0:
                        std::cout << "Voltando ao menu principal." << std::endl;
                        system("cls"); // Limpa a tela do console
                        break;
                    default:
                        std::cout << "Opção inválida." << std::endl;
                        system("cls"); // Limpa a tela do console
                }
            } while (entregaOp != 0);
            break;
        case 0:
            std::cout << "Saindo do programa." << std::endl;
            break;
        default:
            std::cout << "Opção inválida. Tente novamente." << std::endl;
            system("cls"); // Limpa a tela do console
    }

    } while (op != 0);
    // Salva o estado do repositório antes de sair
    salvarDB(repo);
}
