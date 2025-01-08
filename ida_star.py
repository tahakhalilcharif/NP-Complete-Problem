import matplotlib.pyplot as plt

matrix_sizes = [2,3,4,5,6,7,8]
execution_times = [0.0000,0.0000,0.0020,132.6880,14320.00,1032000.00,378691200]

plt.figure(figsize=(10, 6))
plt.plot(matrix_sizes, execution_times, marker='o', color='black', label='Temps de résolution')

plt.yscale('log')
plt.xticks(matrix_sizes)
plt.xlabel('Taille du Taquin (n)', fontsize=12)
plt.ylabel('Temps de Résolution (secondes)', fontsize=12)
plt.title('Temps de Résolution du Taquin en Fonction de la Taille', fontsize=14)

plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.legend(fontsize=12)

plt.show()
