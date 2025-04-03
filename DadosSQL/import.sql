CREATE TABLE Donos (
    nif INTEGER PRIMARY KEY,
    nome TEXT NOT NULL,
    codPostal TEXT NOT NULL
);

CREATE TABLE Veiculos (
    matricula TEXT PRIMARY KEY,
    marca TEXT NOT NULL,
    modelo TEXT NOT NULL,
    ano INTEGER NOT NULL,
    nif INTEGER NOT NULL,
    codVeiculo INTEGER UNIQUE NOT NULL,
    FOREIGN KEY (nif) REFERENCES Donos(nif)
);

CREATE TABLE Sensores (
    codSensor INTEGER PRIMARY KEY,
    designacao TEXT NOT NULL,
    latitude TEXT NOT NULL,
    longitude TEXT NOT NULL
);

CREATE TABLE Distancias (
    codSensor1 INTEGER NOT NULL,
    codSensor2 INTEGER NOT NULL,
    distancia REAL NOT NULL,
    PRIMARY KEY (codSensor1, codSensor2),
    FOREIGN KEY (codSensor1) REFERENCES Sensores(codSensor),
    FOREIGN KEY (codSensor2) REFERENCES Sensores(codSensor)
);

CREATE TABLE Passagens (
    idSensor INTEGER NOT NULL,
    codVeiculo INTEGER NOT NULL,
    dataPassagem TEXT NOT NULL,
    tipoRegisto INTEGER NOT NULL CHECK (tipoRegisto IN (0,1)),
    FOREIGN KEY (idSensor) REFERENCES Sensores(codSensor),
    FOREIGN KEY (codVeiculo) REFERENCES Veiculos(codVeiculo)
);
