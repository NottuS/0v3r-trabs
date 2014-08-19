#!/usr/bin/ruby
# -*- coding: utf-8 -*-
require 'rubygems'
require 'active_record'
require 'active_support/inflector'
require 'active_support/core_ext/string'

DB = "Register.sqlite3"

ActiveRecord::Base.establish_connection :adapter => "sqlite3",
                                        :database => DB

#########CLASSES##############

class Person < ActiveRecord::Base;
    has_and_belongs_to_many :addresses
    has_many                :phones
end

class Address < ActiveRecord::Base;
    has_one                 :city
    has_and_belongs_to_many :people
end     

class City < ActiveRecord::Base;
    belongs_to :address
end

class Phone < ActiveRecord::Base;
    belongs_to :person
end

########CLASSES###########
#######METHODS############

def createDBIfNotExist()
    if File.exist?(DB)
        return 
    end

    ActiveRecord::Base.connection.create_table :addresses do |t|
        t.string       :street
        t.integer      :number
    end

    ActiveRecord::Base.connection.create_table :people do |t|#, :primary_key=>'cpf' do |t|
        t.string        :first_name
        t.string        :last_name
        t.string        :cpf , unique: true, null: false
    end

    ActiveRecord::Base.connection.create_table :cities do |t|
        t.string        :name
        t.references    :address
    end

    ActiveRecord::Base.connection.create_table :phones do |t|
        t.string     :number
        t.references :person
    end

    ActiveRecord::Base.connection.create_table :addresses_people do |t|
        t.references       :person
        t.references       :address
    end
end

def show(table, comand_line, root)

    table.all.each do |t|
        table.column_names.each do |c|
            if (c =~ /_id$/).nil?
                print "#{c.humanize.titleize}: "
                puts "#{t.send(c)}"
            end
        end
        table.reflect_on_all_associations.each do |r|
            if r.name.to_s.singularize.titleize != root.to_s
                if [:has_many, :has_and_belongs_to_many].include?(r.macro)
                    puts "#{r.name.to_s.titleize}:"
                    show(t.send(r.name.to_s), comand_line, table)
                elsif r.macro == :has_one
                    if !t.send(r.name.to_s.singularize).nil?
                        puts "#{r.name.to_s.singularize.titleize}:"
                        aux = Object.const_get(r.name.to_s.singularize.titleize, Class.new)
                        aux.column_names.each do |c|
                            if (c =~ /_id$/).nil?
                                print "#{c.humanize.titleize}: "
                                puts "#{t.send(r.name.to_s.singularize).send(c)}"
                            end
                        end
                    end
                end
            end
        end
        if root.nil?
            puts "------------------------------------------------------------"
        end
    end
end

def getData(table, comand_line)
    data = {}
    if comand_line
        table.column_names.each do |c|
            if (c =~ /id$/).nil?
                data[c] = ARGV[$INDEX]
                $INDEX += 1
            end
        end
    else
        puts "Inform:"
        table.column_names.each do |c|
            if (c =~ /id$/).nil?
                print "#{c.humanize.titleize}: "
                data[c] = $stdin.gets.delete("\n")
            end
        end
    end

    return data
end

def insert(table, comand_line, root)
    data = getData(table, comand_line)
    #puts #{table.name}: #{data}
    t = Object.const_get(table.name, Class.new).where(data)
    if root.nil? or t.empty?
        t = table.create(data)
    else
        return table.kind_of?(Array) ? ( table.include?(t[0]) ? nil : table << t[0]) : t[0]
    end

    n = 0
    table.reflect_on_all_associations.each do |r|
        if r.name.to_s.singularize.titleize != root.to_s
            if [:has_many, :has_and_belongs_to_many].include?(r.macro)
                if comand_line
                    n = ARGV[$INDEX].to_i
                    $INDEX += 1
                else
                    print "Inform the number of #{r.name}: "
                    n = $stdin.gets.delete("\n").to_i
                end
                (1..n).each do |i|
                    insert(t.send(r.name), comand_line, table)
                end
            elsif r.macro == :has_one
                if !comand_line
                    puts "Insert #{r.name.to_s.singularize}:"
                end
                t.send("#{r.name.to_s.singularize}=", 
                    insert(Object.const_get(r.name.to_s.singularize.titleize, Class.new), comand_line, table))
                t.save
            end
        end
    end

    return t
end

def remove(table, comand_line, root)
    remove_by = "id"
    data = {}
    if comand_line
        data["id"] = ARGV[$INDEX]
    else
        data["id"] = $stdin.gets.delete("\n")
    end

    table.destroy_all(data)
end

def update(table, comand_line, root)
    data = {}
    columns = []
    table.content_columns.each{|c| columns << c.name}
    if comand_line
        data["id"] = ARGV[$INDEX]
        if table.find_by_id(data["id"]).nil?
            puts "id not found: #{data["id"]} "
            exit
        end
        data["column"] = ARGV[$INDEX + 1]
        if !columns.include?(data["column"])
            puts "Invalid column!!!"
            exit
        end
        data["value"] = ARGV[$INDEX + 2]
    else
        print "Inform the id: "
        data["id"] = $stdin.gets.delete("\n")
        if table.find_by_id(data["id"]).nil?
            puts "id not found: #{data["id"]} "
            exit
        end
        print "Inform the column #{columns}: "
        data["column"] = $stdin.gets.delete("\n")
        while !columns.include?(data["column"])
            puts "Invalid column!!!"
            print "Inform the column #{columns}: "
            data["column"] = $stdin.gets.delete("\n")
        end

        print "Inform the new value: "
        data["values"] = $stdin.gets.delete("\n")        
    end
    t = table.find_by_id(data["id"])
    if t.send("#{data["column"]}").is_a?(Integer)
        data["value"] = data["value"].to_i
     end
    t.send("#{data["column"]}=", data["value"])
    t.save
end

def help()
    print "Usage: ruby T2.rb operation[insert|remove|show|update|help] table_name["
    ActiveRecord::Base.connection.tables.each do |t|
        print t
        if t == ActiveRecord::Base.connection.tables[-2]
            break
        end
        print "|"
    end
    puts "] [-d|--data]"
    puts "\n\t -d, --data \tthe data that will be used is passed through the command line separated by space(' '), instead of STDIN."
    puts "\t\t Ex: ruby T2 remove people -q 1 , ruby T2 -q alter people -q  1 first_name Luiz"
    exit 
end
#######METHODS############
########INIT##############

createDBIfNotExist()
comand_line = false
$INDEX = 0

if not (ARGV.length > 0 and ['insert', 'remove', 'show', 'update'].include?(ARGV[0])) or ARGV[0] == 'help'
    help
end

if not (ActiveRecord::Base.connection.tables.include?(ARGV[1]) ) #and ARGV[1] != nil)
    puts "ERROR - Table not found: #{ARGV[1]}"
    exit
end

function = ARGV[0]
table = Object.const_get(ARGV[1].singularize.titleize, Class.new)

if ["--data", "-d"].include?(ARGV[2])
    comand_line = true
    $INDEX = 3
end

Object.send(function, table, comand_line, nil)


=begin
ActiveRecord::Base.connection.tables.each do |t| 
    t.column_names.each do |c|
       puts c
   end
end
a = Object.const_get("Address", Class.new)
data = {}
data["street"], data["number"] = "asasd", 345
a = Object.const_get("Person", Class.new).all[4]
puts a.addresses.name

Object.const_get("Address", Class.new).send("create", $PARAMS)
a.send("column_names").each do |c|
    puts c
end
puts "dgshdgfjsgdjh_id".gsub("_id","")
puts "test".pluralize
puts "tests".singularize
puts "test".titleize
puts "last_name".humanize.titleize

puts "Inform" #(CPF, First Name, Last Name, Number of Addresses, Address Info(State, City, Street, Number)):"
puts"CPF:"
cpf = $stdin.gets.delete("\n")

puts"First Name:"
first_name = $stdin.gets.delete("\n")

puts"Last Name:"
last_name = $stdin.gets.delete("\n")

p = Person.create(cpf: cpf, last_name: last_name, first_name: first_name) 

   n = $stdin.gets.delete("\n").to_i
    (1..n).each {
        puts"State:"
        state = $stdin.gets.delete("\n")

        puts"City:"
        city = $stdin.gets.delete("\n")

        puts"Street:"
        street = $stdin.gets.delete("\n")

        puts"Number:"
        number = $stdin.gets.delete("\n").to_i
        p.addresses.create(street: street, number: number).create_city(name: city).create_state(name: state)
    }

                if t.send(c).kind_of?(Array) and nivel < 1
                #puts ""
                #printTable(t.send(c), nivel + 1)
            elsif !(c =~ /_id$/).nil? and nivel < 2
                #printTable(Object.const_get(c.gsub("_id","").titleize, Class.new), nivel + 1)
            else
                puts "#{t.send(c)}"
            end
=end
