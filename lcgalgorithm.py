#LCGALGORITHM

class LinearCongruencialGenerator:
    def __init__(self, seed: int):
        #Los primeros tres parametros, son los que conseguimos en el ransomware
        self._a = 931545873         
        self._c = 101390427
        self._mask = 1073741824

        self._state = seed 

    def next_number(self) -> int:
        #En esta parte hace los calculos y genera el numero al azar
        self._state = (self._a * self._state + self._c) % self._mask 
        return self._state

    def range_pseudorandom_number(self, min_val: int, max_val: int) -> int:
        """
        Esto de aca se me paso explicarlo, y es que como tal LCG genera numeros muy
        grandes, esto para evitar que sean replicables, lo cual obviamente no nos sirve
        si queremos generar un string al azar, porque como maximo los caracteres ascii son 255
        pero LCG genera numeros de 100 mil como minimo (124567), entonces se podran dar a 
        la idea de porque esto es un problema, entonces lo que hacemos es usar la formula de
        abajo, que lo que hace es generar numeros en un rango (0 al 255 por ejemplo), que 
        esta vez si nos sirve para nuestros strings al "azar" 
        """
        range_size = max_val - min_val + 1
        return (self.next_number() % range_size) + min_val
CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"