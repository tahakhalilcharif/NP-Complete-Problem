import matplotlib.pyplot as plt

# Charger les données du fichier texte
def load_data(file_path):
    sizes = []
    times = []
    
    with open(file_path, 'r') as file:
        for line in file:
            size, time = line.strip().split()
            sizes.append(int(size))
            times.append(float(time))
    
    return sizes, times

# Générer et enregistrer le graphique
def generate_and_save_graph(sizes, times, output_path):
    plt.figure(figsize=(10, 6))
    
    # Tracer les données
    plt.plot(sizes, times, marker='o', color='b', label='Temps de résolution')
    
    # Ajouter un titre et des labels aux axes
    plt.title("Temps de Résolution du Taquin en Fonction de la Taille", fontsize=14)
    plt.xlabel("Taille du Taquin (n)", fontsize=12)
    plt.ylabel("Temps de Résolution (secondes)", fontsize=12)
    
    # Ajouter une grille pour une meilleure lisibilité
    plt.grid(True)
    
    # Afficher la légende
    plt.legend()
    
    # Sauvegarder le graphique dans un fichier
    plt.savefig(output_path)
    
    # Afficher le graphe
    plt.show()

# Main
def main():
    file_path = 'execution_times.txt'  # Fichier avec les données
    output_path = 'taquin_resolution_time_graph_non_deterministe.png'  # Nom du fichier pour le graphique
    sizes, times = load_data(file_path)
    generate_and_save_graph(sizes, times, output_path)

if __name__ == "__main__":
    main()
