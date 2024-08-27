package main

import (
	"fmt"
)

type Animal interface {
	Eat()
	Move()
	Speak()
}
type Cow struct {}
type Bird struct {}
type Snake struct {}

func (c Cow) Eat() {
	fmt.Println("grass")
}
func (c Cow) Move() {
	fmt.Println("walk")
}
func (c Cow) Speak() {
	fmt.Println("moo")
}


func (c Bird) Eat() {
	fmt.Println("worms")
}
func (c Bird) Move() {
	fmt.Println("fly")
}
func (c Bird) Speak() {
	fmt.Println("peep")
}

func (c Snake) Eat() {
	fmt.Println("mice")
}
func (c Snake) Move() {
	fmt.Println("slither")
}
func (c Snake) Speak() {
	fmt.Println("hsssss")
}


func main() {

	var NewAnimal = make(map[string]Animal)
	var command , name , action string
	for {
		fmt.Print(">")
		fmt.Scan(&command,&name,&action)
		switch command {
			case "newanimal" :
				switch action {
				case "cow" :
					NewAnimal[name] = Cow{}
				case "bird" :
					NewAnimal[name] = Bird{}
				case "sanke" :
					NewAnimal[name] = Snake{}
				default :
					fmt.Println("Error : Animal Not Found. Try <cow> or <bird> or <snake>")
				}
				fmt.Println("Created it!")
			case "query" :
				animal , ok := NewAnimal[name]
				if !ok {
					fmt.Println("Error : 404 NOT FOUND ")
					continue
				}
				switch action {
				case "eat" :
					animal.Eat()
				case "move" :
					animal.Move()
				case "speak":
					animal.Speak()
				default :
					fmt.Println("Error :Action Not Found")
				}
		default :
		fmt.Println("Error.Try command name action")
		}
	}
}
