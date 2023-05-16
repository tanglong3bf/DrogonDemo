/*
 Navicat Premium Data Transfer

 Source Server         : WSL
 Source Server Type    : MariaDB
 Source Server Version : 100807
 Source Host           : 172.31.68.59:3306
 Source Schema         : login_demo_db

 Target Server Type    : MariaDB
 Target Server Version : 100807
 File Encoding         : 65001

 Date: 15/05/2023 19:09:43
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`  (
  `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `password` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `idx_username`(`username`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 9 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES (1, 'admin123', 'E10ADC3949BA59ABBE56E057F20F883E');

SET FOREIGN_KEY_CHECKS = 1;
