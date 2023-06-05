import numpy as np
import math
import pandas as pd


class calc:
    def __init__(self, N, A, sigma_x, sigma_y, dA, dx, dy):
        self.N = N
        self.A = A
        self.sigma_x = sigma_x
        self.sigma_y = sigma_y
        self.dA = dA
        self.dx = dx
        self.dy = dy

    def dNdx(self):
        return (self.dx/self.sigma_x)**2

    def dNdy(self):
        return (self.dy/self.sigma_y)**2

    def dNdA(self):
        return (self.dA / self.A)**2



class crosssection(calc):
    e = 1.602 * math.pow(10,-19) #elementary charge [C]
    t = 6884.824 #life time [s]
    F = 0.0013 #beam intensity [pnA]
    A_Be = 9.012 #Atomic mass of 9Be [g/mol]
    rho_Be = 1.848 #density of 9Be [g/cm3]
    thick = 0.1 #thickness of 9Be [cm]
    N_A = 6.02214 * math.pow(10,23) #Avogadro constant[/mol]

    def __init__(self,N, A, sigma_x, sigma_y, dA, dx, dy):
        super().__init__(N, A, sigma_x, sigma_y, dA, dx, dy)

    def dN(self):
        return self.N*(np.sqrt(super().dNdx() + super().dNdy() + super().dNdA())) 

    def crosssection(self):
        # sigma = Ne/(Ft) * A/(rho x N_A)
        return self.N * self.e * self.A_Be /(self.F * math.pow(10,-9) * self.t * self.rho_Be * self.thick * self.N_A) * math.pow(10,27)

    def cross_error(self):
        return self.crosssection() * self.dN()/self.N

    def print(self):
        print("N +- deltaN = ",self.N," +- ",self.dN())
        print("production cross section : ", self.crosssection(), " +- ", self.cross_error(), " mb")


file = "./fitting_results.csv"
data = pd.read_csv(file, sep=',')
prm = data.to_numpy()

for i in range(prm.shape[0]):
    cross_i = crosssection(prm[i,11], prm[i,1], prm[i,5], prm[i,9], prm[i,2], prm[i,6], prm[i,10])  
    print(prm[i,0])
    cross_i.print()
    print("")

#nuclide (counts, Amp, sigma_x, sigma_y, Amp_err, x_err, y_err)

