from sklearn.metrics import accuracy_score
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
import pandas as pd

df = pd.read_csv("dataset.csv")

X = df[["temperature","humidity"]]
y = df[["humidifier_status"]]

model = LogisticRegression()
model.fit(X, y)