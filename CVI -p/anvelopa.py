

import numpy as np
from scipy.signal import hilbert
from scipy.io import wavfile

# Citirea unui fișier WAV
sample_rate, signal = wavfile.read("39.wav")  # Schimbă cu calea fișierului tău WAV

# Dacă semnalul este stereo, selectăm un singur canal
if len(signal.shape) > 1:
    signal = signal[:, 0]  # Luăm primul canal

# Calcularea anvelopei folosind transformata Hilbert
analytic_signal = hilbert(signal)
amplitude_envelope = np.abs(analytic_signal)

# Detectarea minimelor și maximelor locale
x = np.arange(len(signal))  # Indicii axei X
local_min_max = np.diff(np.sign(np.diff(signal))).nonzero()[0] + 1
local_min = (np.diff(np.sign(np.diff(signal))) > 0).nonzero()[0] + 1
local_max = (np.diff(np.sign(np.diff(signal))) < 0).nonzero()[0] + 1


# Salvarea anvelopei în fișiere
np.savetxt("hilbert_envelope.txt", amplitude_envelope, fmt="%.6f")  # Anvelopa Hilbert
np.savetxt("local_envelope_maxima.txt", signal[local_max], fmt="%.6f")  # Maxime locale


