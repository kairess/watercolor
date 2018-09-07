import cv2
import numpy as np
from imgaug import augmenters as iaa
from skimage.segmentation import felzenszwalb, slic, quickshift, watershed
from skimage.segmentation import mark_boundaries
import matplotlib.pyplot as plt

img = cv2.imread('/Users/visualcamp/Pictures/watercolor/src.jpg') # (709, 945)
# img = cv2.imread('/Users/visualcamp/Pictures/watercolor/8.jpg')
# img = cv2.imread('/Users/visualcamp/Pictures/watercolor/house.jpg')
# img = cv2.imread('/Users/visualcamp/Pictures/Screenshots/1.png')
texture = cv2.imread('/Users/visualcamp/Development/Clanguage/watercolor/watercolor/assets/texture.jpg', cv2.IMREAD_GRAYSCALE) # (768, 1024)
result = img.copy()

# 0. color adjustment
print('adjust color...')
target_mean = (55.49, 10.12, 20.45)
target_stddev = (20.45, 12.26, 14.85)

result_d = result.astype(np.float32) / 255.0
result_lab = cv2.cvtColor(result_d, cv2.COLOR_BGR2Lab)
result_mean, result_stddev = cv2.meanStdDev(result_lab)

dv = cv2.divide(target_stddev, result_stddev)
cv2.imshow('lab', result_lab)
aaa = (result_lab - result_mean.reshape((1, 1, 3)))
cv2.imshow('aaa', aaa)
result_lab = np.matmul(aaa, dv) + target_mean

result = cv2.cvtColor(result_lab.astype(np.uint8), cv2.COLOR_Lab2BGR)

cv2.imshow('color', result)
cv2.waitKey(0)

result = cv2.GaussianBlur(result, (5, 5), 0)

# 1. mean shift filtering
print('mean shift filtering...')
# sp – The spatial window radius.
# sr – The color window radius.
# maxLevel – Maximum level of the pyramid for the segmentation.
result = cv2.pyrMeanShiftFiltering(result, 40, 40, 5)
# cv2.imshow('pyrMeanShiftFiltering', result.astype(np.uint8))
cv2.imwrite('process/01_pyrMeanShiftFiltering.png', result.astype(np.uint8))

# 2. image segmentation
print('segment image...')
# scale(float): Free parameter. Higher means larger clusters.
# sigma(float): Width (standard deviation) of Gaussian kernel used in preprocessing.
# min_size(int): Minimum component size. Enforced using postprocessing.
segments = felzenszwalb(result, scale=70, sigma=1.0, min_size=10)
# segments = slic(img, n_segments=1000, compactness=20, sigma=5)

# plot boundary
# boundaries = mark_boundaries(img, segments)
# cv2.imshow('boundaries', boundaries)
# mat = plt.matshow(segments, fignum=1)
# plt.colorbar(mat, ticks=np.arange(np.min(segments), np.max(segments)+1))
# plt.show()

# 3. fill mean value in the each segment
print('fill mean value...')
for i in range(np.max(segments)):
  logical_segment = segments == i
  segment_img = result[logical_segment]
  result[logical_segment] = np.mean(segment_img, axis=0)
result = cv2.GaussianBlur(result, (5, 5), 0)

# cv2.imshow('segment', result.astype(np.uint8))
cv2.imwrite('process/02_segment.png', result.astype(np.uint8))

# 4. second mean shift filtering
print('second mean shift filtering...')
result = cv2.pyrMeanShiftFiltering(result, 40, 40, 8)
# cv2.imshow('pyrMeanShiftFiltering', result.astype(np.uint8))
cv2.imwrite('process/03_pyrMeanShiftFiltering.png', result.astype(np.uint8))

# 8. transform a bit
print('transform image...')
seq = iaa.Sequential([
  iaa.GaussianBlur(sigma=0.3),
  iaa.PiecewiseAffine(scale=0.005),
  iaa.ElasticTransformation(alpha=0.5, sigma=0.25),
])

result = seq.augment_image(result)

# 9. texture
print('merge texture...')
if texture.shape[0] > result.shape[0]: # crop
  texture = texture[:result.shape[0], :]
elif texture.shape[0] < result.shape[0]: # pad
  texture = np.pad(texture, ((0, result.shape[0] - texture.shape[0]), (0, 0)), mode='wrap')
if texture.shape[1] > result.shape[1]: # crop
  texture = texture[:, :result.shape[1]]
elif texture.shape[1] < result.shape[1]: # pad
  texture = np.pad(texture, ((0, 0), (0, result.shape[1] - texture.shape[1])), mode='wrap')

texture = np.clip(texture, 230, 255)

result = cv2.merge((result, texture))

cv2.imwrite('result.png', result)

# cv2.imshow('img', texture)
# result = cv2.resize(result, (int(img.shape[1] * 0.5), int(img.shape[0] * 0.5)))
# cv2.imshow('result', result)
# cv2.waitKey(0)

print('completed!')