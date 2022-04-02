require "date"

ROOT = Dir.pwd
BUILD_DIR = "#{ROOT}/build"
OBJECT_DIR = "#{BUILD_DIR}/objects"
LIB_DIR = "#{BUILD_DIR}/lib"
SOURCE_DIR = "#{ROOT}/src"
INCLUDE_DIR = "#{ROOT}/include"


NTLIB = "libnt.so"

WPILIB_HOME = "#{ENV["HOME"]}/wpilib/#{Date.today.year}"
ROBORIO = "#{WPILIB_HOME}/roborio"
ROBORIO_RUBY_INCLUDE = "#{ENV["HOME"]}/dev/ruby/build/3.0.3-arm/home/lvuser/ruby/include/ruby-3.0.0"
ROBORIO_RUBY_CONFIG_INCLUDE = "#{ROBORIO_RUBY_INCLUDE}/arm-linux-gnueabi"

def progress(prog, total, message, width: 80)
  width = message.size + 7 if message.size + 7 > 80
  print "\r" + " " * width + "\r"
  print "#{(prog / total).round(2)}% #{message}"
  puts if prog >= total
end

def include_local
  "-I#{INCLUDE_DIR}"
end

def include_system
  paths = ["#{ROBORIO}/arm-frc2022-linux-gnueabi/usr/include"]
  paths.map{|s| "-I#{s}"}.join(" ")
end

def include_ruby
  "-I#{ROBORIO_RUBY_INCLUDE} -I#{ROBORIO_RUBY_CONFIG_INCLUDE}" 
end

def frc_tool(name)
  arm_prefix = "arm-frc#{Date.today.year}-linux-gnueabi-"
  "#{ROBORIO}/bin/#{arm_prefix}#{name}"
end

task :default => [:build]

task :init => [:init_build_space] do 
  mkdir_p SOURCE_DIR unless Dir.exist?(SOURCE_DIR)
  mkdir_p INCLUDE_DIR unless Dir.exist?(INCLUDE_DIR)
end

task :build => [:init_build_space, :compile, :link]

task :init_build_space do
  mkdir_p BUILD_DIR unless Dir.exist?(BUILD_DIR)
  mkdir_p OBJECT_DIR unless Dir.exist?(OBJECT_DIR)
  mkdir_p LIB_DIR unless Dir.exist?(LIB_DIR)
  mkdir_p SOURCE_DIR unless Dir.exist?(SOURCE_DIR)
end

task :compile => [:init_build_space] do |task, args|
  Dir["#{SOURCE_DIR}/**/*"].each do |file|
    Rake::Task["compile:#{file}"].invoke
  end
end

namespace :compile do
  rule ".c" => [:init_build_space] do |task|
    file = task.name.delete_prefix("compile:")
    file = "#{Rake.original_dir}/#{file}" unless file.start_with?(SOURCE_DIR)
    output_file = "#{OBJECT_DIR}/#{File.basename(file, ".c")}.o"

    command = "#{frc_tool("gcc")} -fPIC -c #{include_local} #{include_ruby} #{include_system} -o #{output_file} #{file}"
    `#{command}`
    abort unless $?.exitstatus.zero?
  end
end

task :link =>[:init_build_space] do
  objects = Dir["#{OBJECT_DIR}/*.o"]
  command = "#{frc_tool("gcc")} -shared #{objects.join(" ")} -o #{LIB_DIR}/#{NTLIB}"
  `#{command}`
  abort unless $?.exitstatus.zero?
end

task :clean do
  rm_r BUILD_DIR
end
